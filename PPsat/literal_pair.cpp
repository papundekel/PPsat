#include <PPsat/literal_pair.hpp>

PPsat::literal_pair::literal_pair() noexcept
    : variable()
    , positive()
{}

PPsat::literal_pair::literal_pair(std::size_t variable, bool positive) noexcept
    : variable(variable)
    , positive(positive)
{}

PPsat::literal_pair::literal_pair(const literal& other) noexcept
    : variable(other.get_variable())
    , positive(other.is_positive())
{}

bool PPsat::literal_pair::is_positive() const noexcept
{
    return positive;
}

std::size_t PPsat::literal_pair::get_variable() const noexcept
{
    return variable;
}
