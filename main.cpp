#include <arc/container.hpp>
#include <arc/handle.hpp>
#include <arc/handle_generator.hpp>
#include <arc/pack.hpp>
#include <arc/property.hpp>
#include <arc/view.hpp>

#include <ostream>

struct vector3
{
    float x = 0.0f, y = 0.0f, z = 0.0f;

    vector3() = default;
    vector3(float value) : vector3(value, value, value) {}
    vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    vector3(vector3 const&) = default;
    vector3& operator=(vector3 const&) = default;

    vector3& operator+=(vector3 const& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }
};

inline std::ostream& operator<<(std::ostream& os, vector3 const& v)
{
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

struct position_tag;
struct velocity_tag;

using position = arc::property<vector3, position_tag>;
using velocity = arc::property<vector3, velocity_tag>;

//using movable = arc::pack<position, velocity>;

#include <iostream>

int main()
{
    arc::container pluto;
    arc::handle_generator pluto_handles(pluto);

    auto movables = arc::view<position, velocity>(pluto);

    auto bob = pluto_handles.generate();

    //auto jeff = bob;

    position::add(bob);
    velocity::add(bob, -1.0f, 0.0f, 42.0f);

    position::set(bob, 1.0f, 2.0f, 3.0f);
    velocity::set(bob, 0.0f, 5.0f, 0.0f);

    //position::remove(bob);

    arc::for_each(movables, [](vector3& position, vector3 const& velocity)
    {
        position += velocity;
    });

    std::cout << position::get(bob) << std::endl;
    std::cout << velocity::get(bob) << std::endl;
}
