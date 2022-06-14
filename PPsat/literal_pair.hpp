#pragma once
#include <PPsat/literal.hpp>

#include <concepts>

namespace PPsat
{
template <std::unsigned_integral U>
class literal_pair
{
public:
    U variable;

private:
    bool positive;

public:
    literal_pair() = default;    

    literal_pair(U variable, bool positive) noexcept
        : variable(variable)
        , positive(positive)
    {}

    literal_pair(const literal auto& other) noexcept
        : variable(other.variable)
        , positive(other.is_positive())
    {}

    bool is_positive() const noexcept
    {
        return positive;
    }

    literal_pair operator!() const noexcept
    {
        return {variable, !positive};
    }
};

template <typename U>
std::ostream& operator<<(std::ostream& out, const literal_pair<U>& l)
{
    if (!l.is_positive())
    {
        out << "-";
    }

    out << l.variable;

    return out;
}
}
