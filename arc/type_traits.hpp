#ifndef ARC_TYPE_TRAITS_HPP
#define ARC_TYPE_TRAITS_HPP

#include <type_traits>

namespace arc
{

template<typename T>
using void_t = void;

template<typename T>
struct is_reference_to_non_const : std::integral_constant<bool,
        std::is_reference<T>::value &&
        !std::is_const<std::remove_reference_t<T>>::value
    > {};

} // namespace arc

#endif // ARC_TYPE_TRAITS_HPP
