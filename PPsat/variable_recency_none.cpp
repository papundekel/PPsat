#include <PPsat/variable_recency_none.hpp>

void PPsat::variable_recency_none::recency_set(std::size_t)
{}

std::size_t PPsat::variable_recency_none::recency_get() const
{
    return 0;
}
