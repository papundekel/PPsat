#include <PPsat/variable_adjacency.hpp>

#include <algorithm>

void PPsat::variable_adjacency::for_each_clause_containing(
    std::function<void(clause&, bool)> f) const
{
    std::ranges::for_each(adjacency,
                          [&f](auto& pair)
                          {
                              f(pair.first, pair.second);
                          });
}

void PPsat::variable_adjacency::register_containing_clause(clause& clause,
                                                           bool positive)
{
    adjacency.emplace_back(clause, positive);
}
