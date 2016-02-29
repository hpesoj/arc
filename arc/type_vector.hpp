#ifndef ARC_TYPE_VECTOR_HPP
#define ARC_TYPE_VECTOR_HPP

#include <arc/type_traits.hpp>
#include <arc/vector.hpp>

#include <type_traits>

namespace arc
{

template<typename... Ts>
struct type_vector
{
};

template<typename T, typename... Ts>
struct push_front<type_vector<Ts...>, T>
{
    using type = type_vector<T, Ts...>;
};

template<typename T, typename... Ts>
struct push_back<type_vector<Ts...>, T>
{
    using type = type_vector<Ts..., T>;
};

template<typename T, typename... Ts>
struct front<type_vector<T, Ts...>>
{
    using type = T;
};

//template<typename T, typename... Ts>
//struct back<type_vector<Ts..., T>>
//{
    //using type = T;
//};

template<typename... Ts>
struct clear<type_vector<Ts...>>
{
    using type = type_vector<>;
};

template<typename... Ts>
struct size<type_vector<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)>
{
};

template<typename... Ts>
struct empty<type_vector<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts) == 0u>
{
};

template<typename T>
struct parameters
{
    using type = typename parameters<decltype(&T::operator())>::type;
};

template<typename R, typename... Args>
struct parameters<R (*)(Args...)>
{
    using type = type_vector<Args...>;
};

template<typename T, typename R, typename... Args>
struct parameters<R (T::*)(Args...) const>
{
    using type = type_vector<Args...>;
};

template<typename T, typename R, typename... Args>
struct parameters<R (T::*)(Args...)>
{
    using type = type_vector<Args...>;
};

template<typename T, typename = void>
using parameters_t = typename parameters<T>::type;

} // namespace arc

#endif // ARC_TYPE_VECTOR_HPP
