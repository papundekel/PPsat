#include <PPsat/variable_simple.hpp>

void PPsat::variable_simple::for_each_clause_containing(
    std::function<void(clause&, bool)>) const
{}

void PPsat::variable_simple::register_containing_clause(clause& clause,
                                                        bool positive)
{}
