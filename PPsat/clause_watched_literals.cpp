#include "PPsat-base/literal.hpp"
#include "PPsat-base/variable_assignment.hpp"
#include "PPsat/variable_assignable.hpp"
#include <PPsat/clause_watched_literals.hpp>

#include <algorithm>

PPsat::clause_watched_literals::clause_watched_literals(
    PPsat_base::view_any<const PPsat_base::literal> literals)
    : literals(literals.begin(), literals.end())
    , watched1(this->literals.front())
    , watched2(this->literals.size() > 1 ? this->literals[1] : watched1)
{}

void PPsat::clause_watched_literals::for_each(
    std::function<void(PPsat_base::literal)> f) const
{}

std::optional<PPsat_base::literal> PPsat::clause_watched_literals::is_unit()
    const
{
    if (literals.size() != 1)
    {
        return {};
    }

    return *literals.begin();
}

std::pair<bool, std::optional<PPsat_base::literal>>
PPsat::clause_watched_literals::assign(PPsat_base::literal literal_assigned,
                                       bool positive_in_clause)
{
    // const PPsat_base::literal literal_in_clause(literal_assigned,
    //                                             positive_in_clause);

    if (literal_assigned.is_positive() ==
        positive_in_clause /* literal_in_clause.is_positive() */)
    {
        return {false, {}};
    }

    if (/* literal_in_clause */ &literal_assigned.get_variable() ==
        /* watched2 */ &watched2.get_variable())
    {
        std::swap(watched1, watched2);
    }

    bool found_sat = false;
    bool found_unassigned = false;
    std::optional<PPsat_base::literal> other_opt;

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
            found_unassigned = true;

            if (literal != watched2)
            {
                other_opt.emplace(literal);
            }
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

    if (other_opt)
    {
        watched1.unregister(*this);
        watched1 = *other_opt;
        watched1.register_(*this);

        return {false, {}};
    }

    return {false, watched2};
}

void PPsat::clause_watched_literals::unassign(PPsat_base::literal, bool)
{}

bool PPsat::clause_watched_literals::is_relevant(
    PPsat_base::literal literal) const
{
    return literal == watched1 || literal == watched2;
}
