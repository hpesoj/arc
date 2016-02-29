#ifndef ARC_VECTOR_HPP
#define ARC_VECTOR_HPP

namespace arc
{

template<typename Sequence, typename T>
struct push_front {};

template<typename Sequence, typename T>
using push_front_t = typename push_front<Sequence, T>::type;

template<typename Sequence, typename T>
struct push_back {};

template<typename Sequence, typename T>
using push_back_t = typename push_back<Sequence, T>::type;

template<typename Sequence>
struct front {};

template<typename Sequence>
using front_t = typename front<Sequence>::type;

template<typename Sequence>
constexpr auto front_v = front<Sequence>::value;

template<typename Sequence>
struct back {};

template<typename Sequence>
using back_t = typename back<Sequence>::type;

template<typename Sequence>
constexpr auto back_v = back<Sequence>::value;

template<typename Sequence>
struct clear {};

template<typename Sequence>
using clear_t = typename clear<Sequence>::type;

template<typename Sequence>
struct size {};

template<typename Sequence>
constexpr bool size_v = size<Sequence>::value;

template<typename Sequence>
struct empty {};

template<typename Sequence>
constexpr bool empty_v = empty<Sequence>::value;

} // namespace arc

#endif // ARC_VECTOR_HPP
