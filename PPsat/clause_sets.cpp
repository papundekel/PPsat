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
{}

std::optional<PPsat_base::literal> PPsat::clause_sets::is_unit() const
{
    if (!assigned_true.empty() || unassigned.size() != 1)
    {
        return {};
    }

    return *unassigned.begin();
}

std::pair<bool, std::optional<PPsat_base::literal>> PPsat::clause_sets::assign(
    PPsat_base::literal literal_assigned,
    bool positive_in_clause)
{
    const PPsat_base::literal literal_in_clause(literal_assigned,
                                                positive_in_clause);

    unassigned.erase(literal_in_clause);

    (literal_assigned.is_positive() == literal_in_clause.is_positive()
         ? assigned_true
         : assigned_false)
        .emplace(literal_in_clause);

    if (!assigned_true.empty())
    {
        return {false, {}};
    }

    const auto unassigned_count = unassigned.size();
    if (unassigned_count == 0)
    {
        return {true, {}};
    }

    if (unassigned_count != 1)
    {
        return {false, {}};
    }

    return {false, *unassigned.begin()};
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

bool PPsat::clause_sets::is_relevant(PPsat_base::literal literal) const
{
    return true;
}
