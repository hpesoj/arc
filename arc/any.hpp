#pragma once

#include <arc/type_index.hpp>

#include <memory>

namespace arc
{

namespace detail
{

struct any_proxy_base
{
    virtual ~any_proxy_base() {}

    virtual explicit operator bool() const = 0;
    virtual std::unique_ptr<any_proxy_base> copy() const = 0;
};

struct any_proxy_null : any_proxy_base
{
    explicit operator bool() const override
    {
        return false;
    }

    std::unique_ptr<any_proxy_base> copy() const override
    {
        return std::make_unique<any_proxy_null>();
    }
};

template<typename T>
struct any_proxy : any_proxy_base
{
    T value;

    template<typename U>
    any_proxy(U&& v) :
        value(std::forward<U>(v))
    {
    }

    explicit operator bool() const override
    {
        return true;
    }

    std::unique_ptr<any_proxy_base> copy() const override
    {
        return std::make_unique<any_proxy>(value);
    }
};

struct any_impl
{
    std::unique_ptr<any_proxy_base> proxy;
    type_index_t type_index_;

    any_impl() :
        proxy(std::make_unique<any_proxy_null>()),
        type_index_()
    {
    }

    template<typename T>
    any_impl(T&& value) :
        proxy(std::make_unique<any_proxy<T>>(std::forward<T>(value))),
        type_index_(type_index<T>())
    {
    }

    template<typename T>
    any_impl& operator=(T&& value)
    {
        proxy = std::make_unique<any_proxy<T>>(std::forward<T>(value));
        type_index_ = type_index<T>();

        return *this;
    }

    any_impl(any_impl const& other) :
        proxy(other.proxy->copy()),
        type_index_(other.type_index_)
    {
    }

    any_impl& operator=(any_impl const& other)
    {
        proxy = other.proxy->copy();
        type_index_ = other.type_index_;

        return *this;
    }

    any_impl(any_impl&& other) :
        proxy(std::move(other.proxy)),
        type_index_(other.type_index_)
    {
        other.type_index_ = type_index_t();
    }

    any_impl& operator=(any_impl&& other)
    {
        proxy = std::move(other.proxy);
        type_index_ = other.type_index_;
        other.type_index_ = type_index_t();

        return *this;
    }

    explicit operator bool() const
    {
        return static_cast<bool>(*proxy);
    }

    template<typename T>
    friend T const* any_cast_impl(any_impl const* a)
    {
        return type_index<T>() == a->type_index_ ? &static_cast<any_proxy<T> const&>(*a->proxy).value : nullptr;
    }

    // TODO unconstify
    template<typename T>
    friend T* any_cast_impl(any_impl* a)
    {
        return type_index<T>() == a->type_index_ ? &static_cast<any_proxy<T>&>(*a->proxy).value : nullptr;
    }
};

} // namespace detail

class any
{
private:
    detail::any_impl m;

public:
    any() = default;

    template<typename T>
    any(T&& value) :
        m(std::forward<T>(value))
    {
    }

    any(any const&) = default;
    any& operator=(any const&) = default;

    any(any&&) = default;
    any& operator=(any&&) = default;

    explicit operator bool() const
    {
        return static_cast<bool>(m);
    }

    template<typename T>
    friend T const* any_cast(any const* a)
    {
        return detail::any_cast_impl<T>(&a->m);
    }

    template<typename T>
    friend T* any_cast(any* a)
    {
        return detail::any_cast_impl<T>(&a->m);
    }
};

} // namespace arc
