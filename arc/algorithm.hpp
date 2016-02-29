#ifndef ARC_ALGORITHM_HPP
#define ARC_ALGORITHM_HPP

#include <arc/type_vector.hpp>
#include <arc/value_vector.hpp>

namespace arc
{

template<typename Sequence, typename T, bool Bool>
struct push_front_if
{
    using type = push_front_t<Sequence, T>;
};

template<typename Sequence, typename T>
struct push_front_if<Sequence, T, false>
{
    using type = Sequence;
};

template<typename Sequence, typename T, bool Condition>
using push_front_if_t = typename push_front_if<Sequence, T, Condition>::type;

template<typename Sequence, template<typename> typename Function, typename ValueType = typename Function<int>::value_type>
struct map {};

template<typename Sequence, template<typename> typename Function, typename ValueType = typename Function<int>::value_type>
using map_t = typename map<Sequence, Function, ValueType>::type;

template<template<typename> typename Function, typename ValueType, typename T, typename... Ts>
struct map<type_vector<T, Ts...>, Function, ValueType>
{
    using type = push_front_t<map_t<type_vector<Ts...>, Function, ValueType>, value<typename Function<T>::value_type, Function<T>::value>>;
};

template<template<typename> typename Function, typename ValueType>
struct map<type_vector<>, Function, ValueType>
{
    using type = value_vector<ValueType>;
};

template<typename Sequence, template<typename> typename Predicate>
struct filter {};

template<typename Sequence, template<typename> typename Predicate>
using filter_t = typename filter<Sequence, Predicate>::type;

template<template<typename> typename Predicate, typename T, typename... Ts>
struct filter<type_vector<T, Ts...>, Predicate>
{
    using type = push_front_if_t<filter_t<type_vector<Ts...>, Predicate>, T, Predicate<T>::value>;
};

template<template<typename> typename Predicate>
struct filter<type_vector<>, Predicate>
{
    using type = type_vector<>;
};

template<typename Sequence, typename BoolSequence>
struct compress {};

template<typename Sequence, typename BoolSequence>
using compress_t = typename compress<Sequence, BoolSequence>::type;

template<typename T, typename... Ts, bool B, bool... Bs>
struct compress<type_vector<T, Ts...>, value_vector<bool, B, Bs...>>
{
    using type = push_front_if_t<compress_t<type_vector<Ts...>, value_vector<bool, Bs...>>, T, B>;
};

template<>
struct compress<type_vector<>, value_vector<bool>>
{
    using type = type_vector<>;
};

} // namespace arc

#endif // ARC_ALGORITHM_HPP
