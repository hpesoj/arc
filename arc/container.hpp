#ifndef ARC_CONTAINER_HPP
#define ARC_CONTAINER_HPP

#include <arc/any.hpp>
#include <arc/id.hpp>
#include <arc/tag.hpp>
#include <arc/type_index.hpp>

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace arc
{

class container
{
public:
    struct observer
    {
        virtual ~observer() {}
        virtual void on_value_added(container&, id_t) = 0;
        virtual void on_value_removed(container&, id_t) = 0;
        virtual void on_value_changed(container&, id_t) = 0;
    };

private:
    template<typename T>
    struct value_data
    {
        std::unordered_map<id_t, tag_t<T>> values;
        std::vector<observer*> observers;
    };

    std::unordered_map<type_index_t, any> m_value_data;

public:
    template<typename T, typename... Args>
    tag_t<T>& add(id_t id, Args&&... args)
    {
        auto& a = m_value_data[type_index<T>()];

        using data_t = value_data<T>;

        if (!a)
        {
            a = data_t();
        }

        auto data = any_cast<data_t>(&a);

        auto& value = data->values.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(id),
                std::forward_as_tuple(std::forward<Args>(args)...)
            ).first->second;

        for (auto& o : data->observers)
        {
            o->on_value_added(*this, id);
        }

        return value;
    }

    template<typename T>
    void remove(id_t id)
    {
        auto index = type_index<T>();
        auto& a = m_value_data[index];

        using data_t = value_data<T>;

        if (a)
        {
            auto data = any_cast<data_t>(&a);

            data->values.erase(id);

            if (!data->observers.empty())
            {
                for (auto& o : data->observers)
                {
                    o->on_value_removed(*this, id);
                }
            }
            else if (data->values.empty())
            {
                m_value_data.erase(index);
            }
        }
    }

    template<typename T, typename Arg, typename... Args>
    void set(id_t id, Arg&& arg, Args&&... args)
    {
        auto& a = m_value_data.find(type_index<T>())->second;
        auto data = any_cast<value_data<T>>(&a);
        data->values.find(id)->second = { std::forward<Arg>(arg), std::forward<Args>(args)... };

        for (auto& o : data->observers)
        {
            o->on_value_changed(*this, id);
        }
    }

    template<typename T>
    tag_t<T> const& get(id_t id) const
    {
        auto& a = m_value_data.find(type_index<T>())->second;
        auto data = any_cast<value_data<T>>(&a);
        return data->values.find(id)->second;
    }

public:
    template<typename T>
    void add_observer(observer& o)
    {
        auto& a = m_value_data[type_index<T>()];

        using data_t = value_data<T>;

        if (!a)
        {
            a = data_t();
        }

        auto data = any_cast<data_t>(&a);

        data->observers.push_back(&o);
    }

    template<typename T>
    void remove_observer(observer& o)
    {
        auto index = type_index<T>();
        auto& a = m_value_data[index];

        using data_t = value_data<T>;

        if (a)
        {
            auto data = any_cast<data_t>(&a);

            auto pos = std::find(data->observers.rbegin(), data->observers.rend(), &o);

            if (pos != data->observers.rend())
            {
                data->observers.erase(std::next(pos).base());
            }

            if (data->observers.empty() && data->values.empty())
            {
                m_value_data.erase(index);
            }
        }
    }
};

} // namespace arc

#endif // ARC_CONTAINER_HPP
