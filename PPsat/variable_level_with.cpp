#include <PPsat/variable_level_with.hpp>

void PPsat::variable_level_with::level_set(std::size_t level)
{
    this->level = level;
}

std::size_t PPsat::variable_level_with::level_get() const
{
    return level;
}
