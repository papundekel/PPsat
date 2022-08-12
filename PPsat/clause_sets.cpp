#include <PPsat/clause_sets.hpp>

#include <algorithm>
#include <bits/ranges_algo.h>

PPsat::clause_sets::clause_sets(view_any<const literal> literals)
    : unassigned(literals.begin(), literals.end())
    , assigned_false()
    , assigned_true()
{}

void PPsat::clause_sets::for_each(std::function<void(literal)> f) const
{
    std::ranges::for_each(unassigned, f);
    std::ranges::for_each(assigned_false, f);
    std::ranges::for_each(assigned_true, f);
}

std::size_t PPsat::clause_sets::length() const noexcept
{
    return unassigned.size() + assigned_false.size() + assigned_true.size();
}

bool PPsat::clause_sets::is_sat() const noexcept
{
    return !assigned_true.empty();
}

std::pair<PPsat::clause::category, PPsat::literal>
PPsat::clause_sets::get_category_and_first_literal_impl() const noexcept
{
    const auto unassigned_count = unassigned.size();
    if (unassigned_count == 0)
    {
        return {clause::category::unsat, {}};
    }
    else
    {
        return {unassigned_count == 1 ? clause::category::unit
                                      : clause::category::other,
                *unassigned.begin()};
    }
}

void PPsat::clause_sets::assign(literal literal_assigned,
                                bool positive_in_clause)
{
    unassigned.erase(literal{literal_assigned, positive_in_clause});

    if (literal_assigned.is_positive() == positive_in_clause)
    {
        assigned_true.emplace(literal_assigned);
    }
    else
    {
        assigned_false.emplace(literal_assigned);
    }
}

void PPsat::clause_sets::unassign(literal literal_unassigned,
                                  bool positive_in_clause)
{
    if (literal_unassigned.is_positive() == positive_in_clause)
    {
        assigned_true.erase(literal_unassigned);
    }
    else
    {
        assigned_false.erase(!literal_unassigned);
    }

    unassigned.emplace(literal{literal_unassigned, positive_in_clause});
}
