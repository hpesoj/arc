#ifndef ARC_PROPERTY_HPP
#define ARC_PROPERTY_HPP

#include <arc/pack.hpp>
#include <arc/tag.hpp>

#include <tuple>
#include <type_traits>
#include <unordered_map>

namespace arc
{

template<typename T, typename Tag>
struct property;

template<typename T, typename Tag>
struct tag<property<T, Tag>>
{
    using type = typename property<T, Tag>::type;
};

template<typename T, typename Tag>
class property final
{
private:
    class accessor
    {
    private:
        handle* m_handle;

    public:
        accessor(handle& h) :
            m_handle(&h)
        {
        }

        accessor(accessor const&) = default;
        accessor& operator=(accessor const&) = default;

        template<typename U, typename = std::enable_if_t<std::is_convertible<U, tag_t<property>>::value>>
        accessor& operator=(U&& value)
        {
            m_handle->set<property>(std::forward<U>(value));
            return *this;
        }

        operator tag_t<property> const&() const
        {
            return m_handle->get<property>();
        }
    };

    struct const_accessor
    {
        handle const* m_handle;

        const_accessor(handle const& h) :
            m_handle(&h)
        {
        }

        const_accessor(const_accessor const&) = default;
        const_accessor& operator=(const_accessor const&) = default;

        operator tag_t<property> const&() const
        {
            return m_handle->get<property>();
        }
    };

public:
    using type = T;

    template<typename... Args>
    static T& add(handle& h, Args&&... args)
    {
        return h.add<property>(std::forward<Args>(args)...);
    }

    static void remove(handle& h)
    {
        return h.remove<property>();
    }

    template<typename Arg, typename... Args>
    static void set(handle& h, Arg&& arg, Args&&... args)
    {
        return h.set<property>(std::forward<Arg>(arg), std::forward<Args>(args)...);
    }

    static T const& get(handle const& h)
    {
        return h.get<property>();
    }

    static accessor of(handle& h)
    {
        return h;
    }

    static const_accessor of(handle const& h)
    {
        return h;
    }

    template<typename... Ts>
    static T& of(pack<Ts...>& p)
    {
        return p.template get<property>();
    }

    template<typename... Ts>
    static T const& of(pack<Ts...> const& p)
    {
        return p.template get<property>();
    }
};

} // namespace arc

#endif // ARC_PROPERTY_HPP
