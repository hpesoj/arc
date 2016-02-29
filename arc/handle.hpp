#ifndef ARC_HANDLE_HPP
#define ARC_HANDLE_HPP

#include <arc/container.hpp>
#include <arc/id.hpp>

#include <vector>
#include <unordered_map>

namespace arc
{

class handle
{
private:
    container* m_container;
    id_t m_id;

public:
    handle(container& c, id_t id) :
        m_container(&c),
        m_id(id)
    {
    }

    handle(handle&& other)
    {
    }

    handle& operator=(handle&& other)
    {
        return *this;
    }

    template<typename T, typename... Args>
    tag_t<T>& add(Args&&... args)
    {
        return m_container->add<T>(m_id, std::forward<Args>(args)...);
    }

    template<typename T>
    void remove()
    {
        m_container->remove<T>(m_id);
    }

    template<typename T, typename Arg, typename... Args>
    void set(Arg&& arg, Args&&... args)
    {
        m_container->set<T>(m_id, std::forward<Arg>(arg), std::forward<Args>(args)...);
    }

    template<typename T>
    tag_t<T> const& get() const
    {
        return m_container->get<T>(m_id);
    }
};

} // namespace arc

#endif // ARC_HANDLE_HPP
