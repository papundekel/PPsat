#include <PPsat/adjacency_list.hpp>

#include <PPsat-base/literal.hpp>

#include <algorithm>

void PPsat::adjacency_list::adjacent_for_each(
    std::function<void(PPsat_base::clause&, bool)> f) const
{
    for (auto i = adjacency.begin(); i != adjacency.end();)
    {
        auto& pair = *i++;
        f(pair.first, pair.second);
    }
}

void PPsat::adjacency_list::adjacent_add(PPsat_base::clause& clause,
                                         bool positive)
{
    adjacency.emplace_back(clause, positive);
}

void PPsat::adjacency_list::adjacent_remove(PPsat_base::clause& clause,
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
