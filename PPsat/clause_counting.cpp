#include "PPsat/clause_counting.hpp"
#include <PPsat/clause_sets.hpp>

#include <algorithm>

PPsat::clause_counting::clause_counting(
    PPsat_base::view_any<const PPsat_base::literal> literals)
    : literals(literals.begin(), literals.end())
    , unassigned(this->literals.size())
    , assigned_false(0)
    , assigned_true(0)
{
    if (!this->literals.empty())
    {
        literal_unit = this->literals.front();
    }
}

void PPsat::clause_counting::for_each(
    std::function<void(PPsat_base::literal)> f) const
{
    std::ranges::for_each(literals, f);
}

std::size_t PPsat::clause_counting::length() const noexcept
{
    return literals.size();
}

bool PPsat::clause_counting::is_sat() const noexcept
{
    return literals.size() == 0 || assigned_true != 0;
}

std::pair<PPsat_base::clause::category, PPsat_base::literal>
PPsat::clause_counting::get_category_and_first_literal_impl() const noexcept
{
    if (unassigned == 0)
    {
        return {category::unsat, {}};
    }
    else
    {
        return {unassigned == 1 ? category::unit : category::other,
                literal_unit};
    }
}

void PPsat::clause_counting::assign(PPsat_base::literal literal_assigned,
                                    bool positive_in_clause)
{
    --unassigned;

    if (literal_assigned.is_positive() == positive_in_clause)
    {
        ++assigned_true;
    }
    else
    {
        ++assigned_false;
        for (auto l : literals)
        {
            const auto assignment = l.get_variable().get_assignment();
            if (assignment == PPsat_base::variable::assignment::unknown)
            {
                literal_unit = l;
            }
        }
    }
}

void PPsat::clause_counting::unassign(PPsat_base::literal literal_unassigned,
                                      bool positive_in_clause)
{
    if (literal_unassigned.is_positive() == positive_in_clause)
    {
        --assigned_true;
    }
    else
    {
        --assigned_false;
    }

    ++unassigned;
    literal_unit = {literal_unassigned, positive_in_clause};
}
