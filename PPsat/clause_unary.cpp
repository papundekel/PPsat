#include "PPsat-base/variable_assignment.hpp"
#include <PPsat/clause_unary.hpp>

#include <PPsat-base/literal.hpp>

#include <algorithm>
#include <ranges>

PPsat::clause_unary::clause_unary(PPsat_base::literal literal)
    : literal(literal)
{}

void PPsat::clause_unary::for_each(
    std::function<void(PPsat_base::literal)> f) const
{
    f(literal);
}

PPsat_base::optional<PPsat_base::literal> PPsat::clause_unary::is_unary_unit()
    const
{
    if (literal.assignment_get() != PPsat_base::variable_assignment::unknown)
    {
        return {};
    }

    return literal;
}

std::pair<bool, std::optional<PPsat_base::literal>> PPsat::clause_unary::assign(
    PPsat_base::literal literal_assigned,
    bool positive_in_clause)
{
    return {literal_assigned.is_positive() != positive_in_clause, {}};
}

void PPsat::clause_unary::unassign(PPsat_base::literal literal_unassigned,
                                   bool positive_in_clause)
{}

bool PPsat::clause_unary::is_relevant(PPsat_base::literal literal) const
{
    return true;
}

bool PPsat::clause_unary::antecedent_to_some() const
{
    for (const auto& antecedent : literal.antecedent_get())
    {
        return &antecedent == this;
    }

    return false;
}

void PPsat::clause_unary::unregister()
{
    literal.unregister(*this);
}

std::size_t PPsat::clause_unary::length() const
{
    return 1;
}
