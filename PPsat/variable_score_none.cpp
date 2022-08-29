#include <PPsat/variable_score_none.hpp>

void PPsat::variable_score_none::score_set(double score)
{}

double PPsat::variable_score_none::score_get() const
{
    return 0.;
}
