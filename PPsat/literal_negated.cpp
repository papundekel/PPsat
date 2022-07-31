#include <PPsat/literal_negated.hpp>

PPsat::literal_negated::literal_negated(const literal& l) noexcept
    : l(l)
{}

bool PPsat::literal_negated::is_positive() const noexcept
{
    return !l.is_positive();
}

std::size_t PPsat::literal_negated::get_variable() const noexcept
{
    return l.get_variable();
}

PPsat::literal_negated PPsat::operator!(const literal& l) noexcept
{
    return literal_negated(l);
}
