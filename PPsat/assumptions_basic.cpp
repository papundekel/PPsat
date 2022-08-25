#include <PPsat/assumptions_basic.hpp>

#include <algorithm>

void PPsat::assumptions_basic::for_each(
    std::function<void(PPsat_base::literal)> f) const
{
    std::ranges::for_each(storage, f);
}

void PPsat::assumptions_basic::add(PPsat_base::literal literal)
{
    storage.push_back(literal);
}
