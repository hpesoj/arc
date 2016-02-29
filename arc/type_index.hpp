#pragma once

#include <cstddef>
#include <type_traits>

namespace arc
{

using type_index_t = std::size_t;

namespace detail
{

template<typename T>
struct type_index_proxy
{
    static type_index_t const value;
};

template<typename T>
type_index_t const type_index_proxy<T>::value = reinterpret_cast<type_index_t>(&value);

} // namespace detail

template<typename T>
type_index_t type_index()
{
    return detail::type_index_proxy<std::remove_cv_t<std::remove_reference_t<T>>>::value;
}

} // namespace arc
