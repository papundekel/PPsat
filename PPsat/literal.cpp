#include <PPsat/literal.hpp>

#include <iostream>

std::ostream& PPsat::operator<<(std::ostream& out, const literal& l)
{
    if (!l.is_positive())
    {
        out << "-";
    }

    out << l.get_variable();

    return out;
}

std::strong_ordering PPsat::literal::operator<=>(
    const literal& other) const noexcept
{
    auto compare_variable = get_variable() <=> other.get_variable();
    if (compare_variable != 0)
    {
        return compare_variable;
    }

    return is_positive() <=> other.is_positive();
}
