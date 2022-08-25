#include <PPsat/clause_basic.hpp>

#include <PPsat-base/variable_assignment.hpp>

#include <algorithm>
#include <bits/ranges_algo.h>

PPsat::clause_basic::clause_basic(
    PPsat_base::view_any<PPsat_base::literal> literals)
    : literals(literals.begin(), literals.end())
{}

void PPsat::clause_basic::for_each(
    std::function<void(PPsat_base::literal)> f) const
{
    std::ranges::for_each(literals, f);
}

PPsat_base::optional<PPsat_base::literal> PPsat::clause_basic::is_unary_unit()
    const
{
    if (literals.size() != 1)
    {
        return {};
    }

    return literals | std::views::filter(
                          [](const PPsat_base::literal literal)
                          {
                              return literal.get_assignment() ==
                                     PPsat_base::variable_assignment::unknown;
                          });
}

std::pair<bool, std::optional<PPsat_base::literal>> PPsat::clause_basic::assign(
    PPsat_base::literal literal_assigned,
    bool positive_in_clause)
{
    const PPsat_base::literal literal_in_clause(literal_assigned,
                                                positive_in_clause);

    if (literal_assigned.is_positive() == literal_in_clause.is_positive())
    {
        return {false, {}};
    }

    bool found_sat = false;
    bool found_unassigned = false;
    bool found_unassigned_multiple = false;
    std::optional<PPsat_base::literal> literal_unit_opt;

    for (const auto literal : literals)
    {
        const auto assignment = literal.get_assignment();
        if (assignment == PPsat_base::variable_assignment::positive)
        {
            found_sat = true;
            break;
        }
        else if (assignment == PPsat_base::variable_assignment::unknown)
        {
            if (found_unassigned)
            {
                found_unassigned_multiple = true;
            }
            found_unassigned = true;
            literal_unit_opt.emplace(literal);
        }
    }

    if (found_sat)
    {
        return {false, {}};
    }

    if (!found_unassigned)
    {
        return {true, {}};
    }

    if (found_unassigned_multiple)
    {
        return {false, {}};
    }

    return {false, *literal_unit_opt};
}

void PPsat::clause_basic::unassign(PPsat_base::literal literal_unassigned,
                                   bool positive_in_clause)
{}

bool PPsat::clause_basic::is_relevant(PPsat_base::literal literal) const
{
    return true;
}
