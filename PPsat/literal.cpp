#include <PPsat/literal.hpp>

#include <PPsat/clause.hpp>
#include <PPsat/unit.hpp>
#include <PPsat/variable.hpp>

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

PPsat::variable_assignment PPsat::literal::assignment_get() const noexcept
{
    const auto assignment_variable = var->assignment_get();
    return positive ? assignment_variable : !assignment_variable;
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

    literal.get_variable().representation_print(output);

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

void PPsat::literal::register_(clause& clause) const
{
    get_variable().register_(clause, positive);
}
void PPsat::literal::unregister(clause& clause) const
{
    get_variable().unregister(clause, positive);
}

std::tuple<PPsat_base::optional<const PPsat::clause&>,
           std::list<PPsat::unit>,
           std::size_t>
PPsat::literal::assign(std::size_t level, std::size_t recency) const
{
    return get_variable().assign(positive, level, recency);
}

void PPsat::literal::unassign() const
{
    get_variable().unassign(positive);
}

std::size_t PPsat::literal::level_get() const
{
    return get_variable().level_get();
}

std::size_t PPsat::literal::recency_get() const
{
    return get_variable().recency_get();
}

PPsat_base::optional<const PPsat::clause&> PPsat::literal::antecedent_get()
    const
{
    return get_variable().antecedent_get();
}
