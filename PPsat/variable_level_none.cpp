#include <PPsat/variable_level_none.hpp>

void PPsat::variable_level_none::level_set(std::size_t)
{}

std::size_t PPsat::variable_level_none::level_get() const
{
    return 0;
}
