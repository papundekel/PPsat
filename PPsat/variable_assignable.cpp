#include "PPsat/variable_assignable.hpp"
#include "PPsat-base/literal.hpp"

#include <algorithm>
#include <bits/ranges_algo.h>

PPsat::variable_assignable::variable_assignable() noexcept
    : assignment_current(assignment::unknown)
{}

void PPsat::variable_assignable::set_assignment(assignment assignment)
{
    assignment_current = assignment;
}

PPsat_base::variable::assignment PPsat::variable_assignable::get_assignment()
    const
{
    return assignment_current;
}
