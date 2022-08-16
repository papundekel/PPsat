#include <PPsat-base/variable_representation_maybe.hpp>

bool PPsat_base::variable_representation_maybe::representation_has() const
{
    return representation_present;
}
void PPsat_base::variable_representation_maybe::representation_set_impl()
{
    representation_present = true;
}
