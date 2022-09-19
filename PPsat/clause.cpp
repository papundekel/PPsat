#include <PPsat/clause.hpp>

bool PPsat::operator==(clause::category category, decisive_t) noexcept
{
    return category == clause::category::sat ||
           category == clause::category::unsat;
}
