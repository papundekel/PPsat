#include <PPsat/formula_simple.hpp>

PPsat::clause_simple::clause_simple(view_any<const literal> literals)
    : literals(literals.begin(), literals.end())
{}

void PPsat::clause_simple::for_each(std::function<void(const literal&)> f) const
{
    std::ranges::for_each(literals, f);
}

std::size_t PPsat::clause_simple::length() const noexcept
{
    return literals.size();
}

void PPsat::formula_simple::add_clause(view_any<const literal> literals)
{
    clauses.emplace_back(literals);
}

void PPsat::formula_simple::for_each(std::function<void(const clause&)> f) const
{
    std::ranges::for_each(clauses, f);
}

std::size_t PPsat::formula_simple::clause_count() const noexcept
{
    return clauses.size();
}
