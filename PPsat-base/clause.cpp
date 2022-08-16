#include <PPsat-base/clause.hpp>

bool PPsat_base::operator==(clause::category category, decisive_t) noexcept
{
    return category == clause::category::sat ||
           category == clause::category::unsat;
}

std::pair<PPsat_base::clause::category, PPsat_base::literal>
PPsat_base::clause::get_category_and_first_literal() const noexcept
{
    return is_sat() ? std::make_pair(category::sat, PPsat_base::literal())
                    : get_category_and_first_literal_impl();
}
