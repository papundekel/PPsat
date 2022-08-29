#include <PPsat/variable_score_with.hpp>

PPsat::variable_score_with::variable_score_with() noexcept
    : score(0)
{}

void PPsat::variable_score_with::score_set(double score)
{
    this->score = score;
}

double PPsat::variable_score_with::score_get() const
{
    return score;
}
