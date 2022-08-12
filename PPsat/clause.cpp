#include <PPsat/clause.hpp>

bool PPsat::operator==(clause::category category, decisive_t) noexcept
{
    return category == clause::category::sat ||
           category == clause::category::unsat;
}

std::pair<PPsat::clause::category, PPsat::literal>
PPsat::clause::get_category_and_first_literal() const noexcept
{
    return is_sat() ? std::make_pair(clause::category::sat, literal())
                    : get_category_and_first_literal_impl();
}
