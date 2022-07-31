#pragma once
#include <PPsat/discard.hpp>

namespace PPsat
{
class discard_iterator_t
{
public:
    constexpr auto operator++(int) noexcept
    {
        return *this;
    }
    constexpr auto operator*() const noexcept
    {
        return discard;
    }
};

constexpr inline discard_iterator_t discard_iterator{};
}
