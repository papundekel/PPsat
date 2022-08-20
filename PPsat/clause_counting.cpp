#include <PPsat/clause_counting.hpp>

#include <PPsat-base/literal.hpp>

#include <algorithm>
#include <iostream>

PPsat::clause_counting::clause_counting(
    PPsat_base::view_any<const PPsat_base::literal> literals)
    : literals(literals.begin(), literals.end())
    , unassigned(this->literals.size())
    , assigned_false(0)
    , assigned_true(0)
{}

void PPsat::clause_counting::for_each(
    std::function<void(PPsat_base::literal)> f) const
{}

std::optional<PPsat_base::literal> PPsat::clause_counting::is_unit() const
{
    if (assigned_true != 0 || unassigned != 1)
    {
        return {};
    }

    std::optional<PPsat_base::literal> literal_unit_opt;

    for (const auto literal : literals)
    {
        const auto assignment = literal.get_assignment();
        if (assignment == PPsat_base::variable::assignment::unknown)
        {
            literal_unit_opt.emplace(literal);
        }
    }

    return literal_unit_opt;
}

std::pair<bool, std::optional<PPsat_base::literal>>
PPsat::clause_counting::assign(PPsat_base::literal literal_assigned,
                               bool positive_in_clause)
{
    --unassigned;

    if (literal_assigned.is_positive() == positive_in_clause)
    {
        ++assigned_true;
        return {false, {}};
    }

    ++assigned_false;

    if (assigned_true != 0)
    {
        return {false, {}};
    }

    if (unassigned == 0)
    {
        return {true, {}};
    }

    if (unassigned != 1)
    {
        return {false, {}};
    };

    const auto i = std::ranges::find_if(
        literals,
        [](PPsat_base::literal literal)
        {
            return literal.get_assignment() ==
                   PPsat_base::variable::assignment::unknown;
        });

    return {false, *i};
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
}

bool PPsat::clause_counting::is_relevant(PPsat_base::literal literal) const
{
    return true;
}
