#include <PPsat-base/literal.hpp>

#include <PPsat-base/variable.hpp>

#include <iostream>

PPsat_base::literal::literal(const literal& literal, bool positive) noexcept
    : var(&literal.get_variable())
    , positive(positive)
{}

PPsat_base::literal::literal(PPsat_base::variable& variable,
                             bool positive) noexcept
    : var(&variable)
    , positive(positive)
{}

PPsat_base::variable& PPsat_base::literal::get_variable() const noexcept
{
    return *var;
}

bool PPsat_base::literal::is_positive() const noexcept
{
    return positive;
}

PPsat_base::literal PPsat_base::literal::operator!() const noexcept
{
    return {*this, !positive};
}

std::ostream& PPsat_base::operator<<(std::ostream& output,
                                     const literal& literal)
{
    if (!literal.is_positive())
    {
        output << "-";
    }

    literal.get_variable().representation_print(output);

    return output;
}

std::strong_ordering PPsat_base::literal::operator<=>(
    const literal& other) const noexcept
{
    auto compare_variable = &get_variable() <=> &other.get_variable();
    if (compare_variable != 0)
    {
        return compare_variable;
    }

    return is_positive() <=> other.is_positive();
}

void PPsat_base::literal::register_containing_clause(clause& clause) const
{
    get_variable().register_containing_clause(clause, positive);
}
