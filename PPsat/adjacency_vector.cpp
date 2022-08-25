#include <PPsat/adjacency_vector.hpp>

#include <PPsat-base/literal.hpp>

#include <algorithm>
#include <bits/ranges_algo.h>

void PPsat::adjacency_vector::adjacent_for_each(
    std::function<void(PPsat_base::clause&, bool)> f) const
{
    std::ranges::for_each(adjacency,
                          [&f](auto& pair)
                          {
                              f(pair.first, pair.second);
                          });
}

void PPsat::adjacency_vector::adjacent_add(PPsat_base::clause& clause,
                                           bool positive)
{
    adjacency.emplace_back(clause, positive);
}

void PPsat::adjacency_vector::adjacent_remove(PPsat_base::clause& clause,
                                              bool positive)
{
    const auto i = std::ranges::find(adjacency,
                                     &clause,
                                     [](const auto& pair)
                                     {
                                         return &pair.first.get();
                                     });

    adjacency.erase(i);
}
std::size_t PPsat::adjacency_vector::size() const
{
    return adjacency.size();
}