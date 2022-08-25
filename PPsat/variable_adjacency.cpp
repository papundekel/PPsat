#include "PPsat/adjacency.hpp"
#include <PPsat/variable_adjacency.hpp>

void PPsat::variable_adjacency::for_each_clause_relevant_assign(
    std::function<void(PPsat_base::clause&, bool)> f) const
{
    adjacent_for_each(std::move(f));
}

void PPsat::variable_adjacency::register_(PPsat_base::clause& clause,
                                          bool positive)
{
    adjacent_add(clause, positive);
}

void PPsat::variable_adjacency::unregister(PPsat_base::clause& clause,
                                           bool positive)
{
    adjacent_remove(clause, positive);
}

std::size_t PPsat::variable_adjacency::adjacency_size() const
{
    return size();
}
