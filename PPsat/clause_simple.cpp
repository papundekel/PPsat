#include <PPsat/clause_simple.hpp>

PPsat::clause_simple::clause_simple(view_any<const literal> literals)
    : literals(literals.begin(), literals.end())
{}

void PPsat::clause_simple::for_each(std::function<void(literal)> f) const
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

std::pair<PPsat::clause::category, PPsat::literal>
PPsat::clause_simple::get_category_and_first_literal_impl() const noexcept
{
    return {clause::category::other, *literals.begin()};
}

void PPsat::clause_simple::assign(literal, bool)
{}

void PPsat::clause_simple::unassign(literal, bool)
{}
