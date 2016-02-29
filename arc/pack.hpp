#ifndef ARC_PACK_HPP
#define ARC_PACK_HPP

#include <arc/tag.hpp>

#include <tuple>
#include <utility>

namespace arc
{

template<typename... Ts>
class pack
{
private:
    template<typename T>
    struct proxy
    {
        tag_t<T> value;

        template<typename U>
        proxy(U&& value) :
            value(std::forward<U>(value))
        {
        }
    };

    std::tuple<proxy<Ts>...> m_values;

public:
    template<typename... Args>
    pack(Args&&... args) :
        m_values(std::forward<Args>(args)...)
    {
    }

    template<typename T>
    tag_t<T> const& get() const
    {
        return std::get<proxy<T>>(m_values).value;
    }

    template<typename T>
    tag_t<T>& get()
    {
        return std::get<proxy<T>>(m_values).value;
    }
};

} // namespace arc

#endif // ARC_PACK_HPP
