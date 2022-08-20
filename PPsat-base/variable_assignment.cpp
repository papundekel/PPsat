#include <PPsat-base/variable_assignment.hpp>

PPsat_base::variable_assignment PPsat_base::operator!(
    variable_assignment assignment)
{
    switch (assignment)
    {
        case variable_assignment::negative:
            return variable_assignment::positive;
        case variable_assignment::positive:
            return variable_assignment::negative;
        default:
            return variable_assignment::unknown;
    }
}
