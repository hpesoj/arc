#ifndef ARC_TAG_HPP
#define ARC_TAG_HPP

namespace arc
{

template<typename T>
struct tag
{
    using type = T;
};

template<typename T>
using tag_t = typename tag<T>::type;

} // namespace arc

#endif // ARC_TAG_HPP
