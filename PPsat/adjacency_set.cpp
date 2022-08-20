#include <PPsat/adjacency_set.hpp>

#include <PPsat-base/literal.hpp>

#include <algorithm>

bool PPsat::adjacency_set::comparer::operator()(
    const element_t& a,
    const element_t& b) const noexcept
{
    return &a.first.get() < &b.first.get();
}

void PPsat::adjacency_set::adjacent_for_each(
    std::function<void(PPsat_base::clause&, bool)> f) const
{
    for (auto i = adjacency.begin(); i != adjacency.end();)
    {
        auto& pair = *i++;
        f(pair.first, pair.second);
    }
}

void PPsat::adjacency_set::adjacent_add(PPsat_base::clause& clause,
                                        bool positive)
{
    adjacency.emplace(clause, positive);
}

void PPsat::adjacency_set::adjacent_remove(PPsat_base::clause& clause,
                                           bool positive)
{
    adjacency.erase(std::make_pair(std::ref(clause), positive));
}
