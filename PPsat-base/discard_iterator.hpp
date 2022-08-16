#pragma once
#include <PPsat-base/discard.hpp>

namespace PPsat_base
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

constexpr inline discard_iterator_t discard_iterator;
}
