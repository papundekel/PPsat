#include <PPsat/variable_simple.hpp>

void PPsat::variable_simple::set_assignment(assignment)
{}

PPsat_base::variable::assignment PPsat::variable_simple::get_assignment() const
{
    return assignment::unknown;
}

void PPsat::variable_simple::for_each_clause_containing(
    std::function<void(PPsat_base::clause&, bool)> f) const
{}

void PPsat::variable_simple::register_containing_clause(
    PPsat_base::clause& clause,
    bool positive)
{}
