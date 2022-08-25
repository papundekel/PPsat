#include "PPsat-base/clause.hpp"
#include "PPsat-base/unit.hpp"
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

PPsat_base::variable_assignment PPsat_base::literal::get_assignment()
    const noexcept
{
    const auto assignment_variable = var->get_assignment();
    return positive ? assignment_variable : !assignment_variable;
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

void PPsat_base::literal::register_(clause& clause) const
{
    get_variable().register_(clause, positive);
}
void PPsat_base::literal::unregister(clause& clause) const
{
    get_variable().unregister(clause, positive);
}

std::tuple<PPsat_base::optional<const PPsat_base::clause&>,
           std::list<PPsat_base::unit>,
           std::size_t>
PPsat_base::literal::assign(std::size_t level, std::size_t recency) const
{
    return get_variable().assign(positive, level, recency);
}

void PPsat_base::literal::unassign() const
{
    get_variable().unassign(positive);
}

std::size_t PPsat_base::literal::level_get() const
{
    return get_variable().level_get();
}

std::size_t PPsat_base::literal::recency_get() const
{
    return get_variable().recency_get();
}

const PPsat_base::clause& PPsat_base::literal::antecedent_get() const
{
    return *get_variable().antecedent_get();
}
