#ifndef ARC_UNORDERED_ANY_MAP_HPP
#define ARC_UNORDERED_ANY_MAP_HPP

#include <arc/any.hpp>
#include <arc/id.hpp>
#include <arc/tag.hpp>
#include <arc/type_index.hpp>

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace arc
{

class unordered_any_map
{
private:
    template<typename T>
    struct map
    {
        std::unordered_map<id_t, tag_t<T>> value;
    };

    std::unordered_map<type_index_t, any> m_maps;

public:
    template<typename T, typename... Args>
    tag_t<T>& add(id_t id, Args&&... args)
    {
        auto& a = m_maps[type_index<T>()];

        if (!a)
        {
            a = map<T>();
        }

        auto& m = any_cast<map<T>>(&a)->value;

        auto& value = m.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(id),
                std::forward_as_tuple(std::forward<Args>(args)...)
            ).first->second;

        for (auto& o : m->observers)
        {
            o->on_value_added(*this, id);
        }

        return value;
    }

    template<typename T>
    void remove(id_t id)
    {
        auto index = type_index<T>();
        auto& a = m_maps[index];

        if (a)
        {
            auto& m = any_cast<map<T>>(&a)->value;

            m.erase(id);

            if (m.empty())
            {
                m_maps.erase(index);
            }
        }
    }

    template<typename T, typename Arg, typename... Args>
    void set(id_t id, Arg&& arg, Args&&... args)
    {
        auto& a = m_maps.find(type_index<T>())->second;
        auto& m = any_cast<map<T>>(&a)->value;
        m.find(id)->second = { std::forward<Arg>(arg), std::forward<Args>(args)... };
    }

    template<typename T>
    tag_t<T> const& get(id_t id) const
    {
        auto& a = m_maps.find(type_index<T>())->second;
        auto& m = any_cast<map<T>>(&a)->value;
        return m.find(id)->second;
    }
};

} // namespace arc

arc::unordered_any_map<int> map;
arc::unordered_any_map_view view(map);

view.emplace();

#endif // ARC_UNORDERED_ANY_MAP_HPP
