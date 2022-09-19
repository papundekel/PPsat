#include <PPsat/assumptions_basic.hpp>

#include <algorithm>

void PPsat::assumptions_basic::for_each(std::function<void(literal)> f) const
{
    std::ranges::for_each(storage, f);
}

void PPsat::assumptions_basic::add(literal literal)
{
    storage.push_back(literal);
}
