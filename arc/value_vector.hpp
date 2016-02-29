#ifndef ARC_VALUE_VECTOR_HPP
#define ARC_VALUE_VECTOR_HPP

#include <type_traits>

namespace arc
{

template<typename T, T Value>
struct value
{
};

template<typename T, T... Values>
struct value_vector
{
    using value_type = T;
};

template<typename T, T... Values, T Value>
struct push_front<value_vector<T, Values...>, value<T, Value>>
{
    using type = value_vector<T, Value, Values...>;
};

template<typename T, T... Values, T Value>
struct push_back<value_vector<T, Values...>, value<T, Value>>
{
    using type = value_vector<T, Values..., Value>;
};

template<typename T, T Value, T... Values>
struct front<value_vector<T, Value, Values...>> : std::integral_constant<T, Value>
{
};

//template<typename T, T... Values>
//struct back<value_vector<T, Values..., Value>> : std::integral_constant<T, Value>
//{
//};

template<typename T, T... Values>
struct clear<value_vector<T, Values...>>
{
    using type = value_vector<T>;
};

template<typename T, T... Values>
struct size<value_vector<T, Values...>> : std::integral_constant<std::size_t, sizeof...(Values)>
{
};

template<typename T, T... Values>
struct empty<value_vector<T, Values...>> : std::integral_constant<std::size_t, sizeof...(Values) == 0u>
{
};

} // namespace arc

#endif // ARC_VALUE_VECTOR_HPP
