#ifndef ARC_VIEW_HPP
#define ARC_VIEW_HPP

#include <arc/algorithm.hpp>
#include <arc/container.hpp>
#include <arc/pack.hpp>
#include <arc/type_vector.hpp>
#include <arc/value_vector.hpp>

#include <tuple>
#include <unordered_map>
#include <vector>

namespace arc
{

struct null_args
{
    template<typename... Args>
    null_args(Args&&...) {}
};

template<typename... Ts>
class view
{
private:
    template<typename T>
    struct container_observer : container::observer
    {
        view* this_;

        container_observer(view* this_) :
            this_(this_)
        {
        }

        void on_value_added(container& c, id_t id) override
        {
            if (++this_->m_counts[id] == sizeof...(Ts))
            {
                this_->m_indices[id] = this_->m_values.size();
                this_->m_ids.push_back(id);
                this_->m_values.emplace_back(c.get<Ts>(id)...);
            }
        }

        void on_value_removed(container&, id_t id) override
        {
            auto index = this_->m_indices[id];

            this_->m_values[index] = std::move(this_->m_values.back());
            this_->m_values.pop_back();

            auto copied_id = this_->m_ids[index] = this_->m_ids.back();
            this_->m_ids.pop_back();

            this_->m_indices[copied_id] = index;
            this_->m_indices.erase(id);

            if (--this_->m_counts[id] == 0)
            {
                this_->m_counts.erase(id);
            }
        }

        void on_value_changed(container& c, id_t id) override
        {
            if (this_->m_counts[id] == sizeof...(Ts))
            {
                auto index = this_->m_indices[id];
                this_->m_values[index].template get<T>() = c.get<T>(id);
            }
        }
    };

    template<typename T>
    friend class container_observer;

    container* m_container;
    std::tuple<container_observer<Ts>...> m_container_observers;
    std::unordered_map<id_t, std::size_t> m_counts;
    std::unordered_map<id_t, std::size_t> m_indices;
    std::vector<id_t> m_ids;
    std::vector<pack<Ts...>> m_values;

public:
    using iterator = typename decltype(m_values)::iterator;
    using const_iterator = typename decltype(m_values)::const_iterator;
    using reverse_iterator = typename decltype(m_values)::reverse_iterator;
    using const_reverse_iterator = typename decltype(m_values)::const_reverse_iterator;

public:
    view(container& c) :
        m_container(&c),
        m_container_observers{((void)static_cast<Ts*>(nullptr), this)...}
    {
        (int[]){ (m_container->add_observer<Ts>(std::get<container_observer<Ts>>(m_container_observers)), 0)... };
    }

    ~view()
    {
        (int[]){ (m_container->remove_observer<Ts>(std::get<container_observer<Ts>>(m_container_observers)), 0)... };
    }

    template<typename... Us>
    void resync()
    {
        std::size_t index = 0u;

        for (auto& p : m_values)
        {
            (int[]){ (m_container->template set<Us>(m_ids[index], p.template get<Us>()), 0)... };
            index += 1u;
        }
    }

    const_iterator begin() const
    {
        return m_values.begin();
    }

    const_iterator end() const
    {
        return m_values.end();
    }

    iterator begin()
    {
        return m_values.begin();
    }

    iterator end()
    {
        return m_values.end();
    }
};

template<typename View, typename... Ts>
class view_modifier
{
private:
    View* m_view;

public:
    using iterator = typename View::iterator;
    using const_iterator = typename View::const_iterator;
    using reverse_iterator = typename View::reverse_iterator;
    using const_reverse_iterator = typename View::const_reverse_iterator;

public:
    view_modifier(View& v) :
        m_view(&v)
    {
    }

    ~view_modifier()
    {
        m_view->template resync<Ts...>();
    }

    //view_modifier(view_modifier const&) = delete;
    //view_modifier& operator=(view_modifier const&) = delete;

    const_iterator begin() const
    {
        return m_view->begin();
    }

    const_iterator end() const
    {
        return m_view->end();
    }

    iterator begin()
    {
        return m_view->begin();
    }

    iterator end()
    {
        return m_view->end();
    }
};

template<typename... Ts, typename... Us>
view_modifier<view<Us...>, Ts...> modify(view<Us...>& v)
{
    return view_modifier<view<Us...>, Ts...>(v);
}

namespace detail
{

template<typename SyncTypes>
struct for_each {};

template<typename... SyncTs>
struct for_each<type_vector<SyncTs...>>
{
    template<typename... Ts, typename Functor>
    static void call(view<Ts...>& v, Functor f)
    {
        for (auto& p : modify(v))
        {
            f(p.template get<Ts>()...);
        }

        v.template resync<std::remove_const_t<SyncTs>...>();
    }
};

} // namespace detail

template<typename Functor, typename... Ts>
void for_each(view<Ts...>& v, Functor f)
{
    using types = type_vector<Ts...>;
    using params = parameters_t<Functor>;
    using sync_types = compress_t<types, map_t<params, is_reference_to_non_const>>;

    detail::for_each<sync_types>::call(v, f);
};

} // namespace arc

#endif // ARC_VIEW_HPP
