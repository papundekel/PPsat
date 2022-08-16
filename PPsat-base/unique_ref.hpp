#pragma once
#include <memory>
#include <utility>

namespace PPsat_base
{
template <typename T>
class unique_ref
{
public:
    std::unique_ptr<T> ptr;

public:
    template <typename U>
    unique_ref(std::unique_ptr<U> ptr) noexcept
        : ptr(std::move(ptr))
    {}
    unique_ref(std::nullptr_t) noexcept
        : ptr(nullptr)
    {}

    operator T&() const
    {
        return *ptr;
    }

    T* operator->() const
    {
        return ptr.get();
    }
};
}
