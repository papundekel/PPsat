
#include <PPsat/variable_assignable_not.hpp>

void PPsat::variable_assignable_not::assignment_set(assignment)
{}

PPsat_base::variable::assignment
PPsat::variable_assignable_not::get_assignment() const
{
    return assignment::unknown;
}
