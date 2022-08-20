#include <PPsat-base/clause.hpp>
#include <optional>

bool PPsat_base::operator==(clause::category category, decisive_t) noexcept
{
    return category == clause::category::sat ||
           category == clause::category::unsat;
}
