#ifndef ARC_ID_GENERATOR_HPP
#define ARC_ID_GENERATOR_HPP

#include <arc/id.hpp>

namespace arc
{

class id_generator
{
private:
    id_t m_next_id;

public:
    id_t generate()
    {
        return m_next_id++;
    }
};

} // namespace arc

#endif // ARC_ID_GENERATOR_HPP
