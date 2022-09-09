#include "PPsat/variable_assignable.hpp"
#include "PPsat-base/literal.hpp"

#include <algorithm>
#include <ranges>

PPsat::variable_assignable::variable_assignable() noexcept
    : assignment_current(assignment::unknown)
{}

void PPsat::variable_assignable::assignment_set(assignment assignment)
{
    assignment_current = assignment;
}

PPsat_base::variable::assignment PPsat::variable_assignable::assignment_get()
    const
{
    return assignment_current;
}
