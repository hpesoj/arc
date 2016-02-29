#ifndef ARC_HANDLE_GENERATOR_HPP
#define ARC_HANDLE_GENERATOR_HPP

#include <arc/container.hpp>
#include <arc/id_generator.hpp>

namespace arc
{

class handle_generator
{
private:
    container* m_container;
    id_generator m_ids;

public:
    handle_generator(container& c) :
        m_container(&c)
    {
    }

    handle generate()
    {
        return handle(*m_container, m_ids.generate());
    }
};

} // namespace arc

#endif // ARC_HANDLE_GENERATOR_HPP
