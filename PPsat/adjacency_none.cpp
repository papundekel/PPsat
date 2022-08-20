#include <PPsat/adjacency_none.hpp>

void PPsat::adjacency_none::adjacent_for_each(
    std::function<void(PPsat_base::clause&, bool)>) const
{}
void PPsat::adjacency_none::adjacent_add(PPsat_base::clause&, bool)
{}
void PPsat::adjacency_none::adjacent_remove(PPsat_base::clause&, bool)
{}
