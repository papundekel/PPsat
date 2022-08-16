#include "PPsat-base/clause.hpp"
#include <PPsat-crypt/clause.hpp>

#include <iostream>

PPsat_crypt::clause::clause(
    PPsat_base::view_any<const PPsat_base::literal> literals)
{
    for (auto l : literals)
    {
        std::cout << l << "\n";
    }
}

void PPsat_crypt::clause::for_each(
    std::function<void(PPsat_base::literal)> f) const
{}
std::size_t PPsat_crypt::clause::length() const noexcept
{
    return 0;
}
void PPsat_crypt::clause::add_literal(PPsat_base::literal literal)
{}
bool PPsat_crypt::clause::is_sat() const noexcept
{
    return false;
}
std::pair<PPsat_base::clause::category, PPsat_base::literal>
PPsat_crypt::clause::get_category_and_first_literal_impl() const noexcept
{
    return {category::other, {}};
}
void PPsat_crypt::clause::assign(PPsat_base::literal literal_assigned,
                                 bool positive_in_clause)
{}
void PPsat_crypt::clause::unassign(PPsat_base::literal literal_unassigned,
                                   bool positive_in_clause)
{}
