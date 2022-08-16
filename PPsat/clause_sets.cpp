#include <PPsat/clause_sets.hpp>

#include <algorithm>

PPsat::clause_sets::clause_sets(
    PPsat_base::view_any<const PPsat_base::literal> literals)
    : unassigned(literals.begin(), literals.end())
    , assigned_false()
    , assigned_true()
{}

void PPsat::clause_sets::for_each(
    std::function<void(PPsat_base::literal)> f) const
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

std::pair<PPsat_base::clause::category, PPsat_base::literal>
PPsat::clause_sets::get_category_and_first_literal_impl() const noexcept
{
    const auto unassigned_count = unassigned.size();
    if (unassigned_count == 0)
    {
        return {category::unsat, {}};
    }
    else
    {
        return {unassigned_count == 1 ? category::unit : category::other,
                *unassigned.begin()};
    }
}

void PPsat::clause_sets::assign(PPsat_base::literal literal_assigned,
                                bool positive_in_clause)
{
    const PPsat_base::literal literal_in_clause(literal_assigned,
                                                positive_in_clause);

    unassigned.erase(literal_in_clause);

    (literal_assigned.is_positive() == literal_in_clause.is_positive()
         ? assigned_true
         : assigned_false)
        .emplace(literal_in_clause);
}

void PPsat::clause_sets::unassign(PPsat_base::literal literal_unassigned,
                                  bool positive_in_clause)
{
    const PPsat_base::literal literal_in_clause(literal_unassigned,
                                                positive_in_clause);

    (literal_unassigned.is_positive() == literal_in_clause.is_positive()
         ? assigned_true
         : assigned_false)
        .erase(literal_in_clause);

    unassigned.emplace(literal_in_clause);
}
