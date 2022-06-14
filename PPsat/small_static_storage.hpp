#pragma once
#include <array>

namespace PPsat
{
template <typename T, std::size_t N>
class small_static_storage
{
    std::array<T, N> storage;
    std::size_t count;

public:
    small_static_storage(auto&&... x)
        : storage{std::forward<decltype(x)>(x)...}
        , count(sizeof...(x))
    {}

    auto begin()
    {
        return storage.begin();
    }
    auto begin() const
    {
        return storage.begin();
    }
    auto end()
    {
        return begin() + count;
    }
    auto end() const
    {
        return begin() + count;
    }
};
}
