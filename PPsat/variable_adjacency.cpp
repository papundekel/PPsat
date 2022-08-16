#include "PPsat-base/literal.hpp"
#include <PPsat/variable_adjacency.hpp>

#include <algorithm>

PPsat::variable_adjacency::variable_adjacency() noexcept
    : assignment_current(assignment::unknown)
    , adjacency()
{}

void PPsat::variable_adjacency::set_assignment(assignment assignment)
{
    assignment_current = assignment;
}

PPsat_base::variable::assignment PPsat::variable_adjacency::get_assignment()
    const
{
    return assignment_current;
}

void PPsat::variable_adjacency::for_each_clause_containing(
    std::function<void(PPsat_base::clause&, bool)> f) const
{
    std::ranges::for_each(adjacency,
                          [&f](auto& pair)
                          {
                              f(pair.first, pair.second);
                          });
}

void PPsat::variable_adjacency::register_containing_clause(
    PPsat_base::clause& clause,
    bool positive)
{
    adjacency.emplace_back(clause, positive);
}
