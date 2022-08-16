#include <PPsat/clause_simple.hpp>

PPsat::clause_simple::clause_simple(
    PPsat_base::view_any<const PPsat_base::literal> literals)
    : literals(literals.begin(), literals.end())
{}

void PPsat::clause_simple::for_each(
    std::function<void(PPsat_base::literal)> f) const
{
    std::ranges::for_each(literals, f);
}

std::size_t PPsat::clause_simple::length() const noexcept
{
    return literals.size();
}

bool PPsat::clause_simple::is_sat() const noexcept
{
    return false;
}

std::pair<PPsat_base::clause::category, PPsat_base::literal>
PPsat::clause_simple::get_category_and_first_literal_impl() const noexcept
{
    return {category::other, *literals.begin()};
}

void PPsat::clause_simple::assign(PPsat_base::literal, bool)
{}

void PPsat::clause_simple::unassign(PPsat_base::literal, bool)
{}
