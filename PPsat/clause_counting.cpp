#include <PPsat/clause_counting.hpp>

#include <PPsat-base/literal.hpp>

#include <algorithm>
#include <ranges>

PPsat::clause_counting::clause_counting(
    PPsat_base::view_any<PPsat_base::literal> literals)
    : literals(literals.begin(), literals.end())
    , unassigned(this->literals.size())
    , assigned_false(0)
    , assigned_true(0)
{}

void PPsat::clause_counting::for_each(
    std::function<void(PPsat_base::literal)> f) const
{
    std::ranges::for_each(literals, f);
}

PPsat_base::optional<PPsat_base::literal>
PPsat::clause_counting::is_unary_unit() const
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
