#include <PPsat/literal.hpp>

#include <iostream>

PPsat::literal::literal(const literal& literal, bool positive) noexcept
    : var(&literal.get_variable())
    , positive(positive)
{}

PPsat::literal::literal(PPsat::variable& variable, bool positive) noexcept
    : var(&variable)
    , positive(positive)
{}

PPsat::variable& PPsat::literal::get_variable() const noexcept
{
    return *var;
}

bool PPsat::literal::is_positive() const noexcept
{
    return positive;
}

PPsat::literal PPsat::literal::operator!() const noexcept
{
    return {*this, !positive};
}

std::ostream& PPsat::operator<<(std::ostream& output, const literal& literal)
{
    if (!literal.is_positive())
    {
        output << "-";
    }

    literal.get_variable().print(output);

    return output;
}

std::strong_ordering PPsat::literal::operator<=>(
    const literal& other) const noexcept
{
    auto compare_variable = &get_variable() <=> &other.get_variable();
    if (compare_variable != 0)
    {
        return compare_variable;
    }

    return is_positive() <=> other.is_positive();
}

void PPsat::literal::register_containing_clause(clause& clause) const
{
    get_variable().register_containing_clause(clause, positive);
}
