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

std::optional<PPsat_base::literal> PPsat::clause_simple::is_unit() const
{
    return {};
}

std::pair<bool, std::optional<PPsat_base::literal>>
PPsat::clause_simple::assign(PPsat_base::literal, bool)
{
    return {false, {}};
}

void PPsat::clause_simple::unassign(PPsat_base::literal, bool)
{}

bool PPsat::clause_simple::is_relevant(PPsat_base::literal literal) const
{
    return false;
}
