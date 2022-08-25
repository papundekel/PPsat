#include "PPsat/variable_recency_with.hpp"

void PPsat::variable_recency_with::recency_set(std::size_t recency)
{
    this->recency = recency;
}

std::size_t PPsat::variable_recency_with::recency_get() const
{
    return recency;
}
