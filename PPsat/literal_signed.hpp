#pragma once
#include <concepts>
#include <iostream>
#include <type_traits>

namespace PPsat
{
template <std::unsigned_integral U>
class literal_signed
{
public:
    std::make_signed_t<U> variable;

public:
    literal_pair(const literal auto& other)
        : variable(other.is_positive() ? other.variable : -other.variable)
    {}

    bool is_positive() const noexcept
    {
        return variable > 0;
    }

    literal_signed operator!() const noexcept
    {
        return {-variable};
    }
};

template <typename U>
std::ostream& operator<<(std::ostream& out, const literal_signed<U>& l)
{
    out << l.variable;

    return out;
}
}
