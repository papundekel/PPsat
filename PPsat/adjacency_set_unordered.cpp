#include <PPsat/adjacency_set_unordered.hpp>

#include <PPsat-base/literal.hpp>

#include <algorithm>

std::size_t PPsat::adjacency_set_unordered::hasher::operator()(
    const element_t& e) const noexcept
{
    return std::hash<decltype(&e.first.get())>()(&e.first.get());
}

bool PPsat::adjacency_set_unordered::comparer::operator()(
    const element_t& a,
    const element_t& b) const noexcept
{
    return &a.first.get() == &b.first.get();
}

void PPsat::adjacency_set_unordered::adjacent_for_each(
    std::function<void(PPsat_base::clause&, bool)> f) const
{
    for (auto i = adjacency.begin(); i != adjacency.end();)
    {
        auto& pair = *i++;
        f(pair.first, pair.second);
    }
}

void PPsat::adjacency_set_unordered::adjacent_add(PPsat_base::clause& clause,
                                                  bool positive)
{
    adjacency.emplace(clause, positive);
}

void PPsat::adjacency_set_unordered::adjacent_remove(PPsat_base::clause& clause,
                                                     bool positive)
{
    adjacency.erase(std::make_pair(std::ref(clause), positive));
}
