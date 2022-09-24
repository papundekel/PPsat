#include <PPsat/conflict_analysis_dpll.hpp>

#include <PPsat/clause.hpp>
#include <PPsat/solver.hpp>
#include <PPsat/unit.hpp>

PPsat::conflict_analysis_dpll::conflict_analysis_dpll(
    const PPsat_base::factory<formula::factory_clause>&,
    decision&) noexcept
{
    reset();
}

void PPsat::conflict_analysis_dpll::reset()
{
    consecutive_second_decisions.clear();
    consecutive_second_decisions.emplace_back(0, 0);
}

std::list<PPsat::unit> PPsat::conflict_analysis_dpll::find_unary_unit() const
{
    return {};
}

PPsat_base::optional<std::size_t> PPsat::conflict_analysis_dpll::analyse(
    std::size_t level,
    const clause& antecedent)
{
    const auto [level_first, level_last] = consecutive_second_decisions.back();

    if (level_last == level)
    {
        consecutive_second_decisions.pop_back();

        if (level_first == 0)
        {
            return {};
        }

        return level_first - 1;
    }

    return level - 1;
}

std::pair<PPsat_base::optional<const PPsat::clause&>, std::list<PPsat::unit>>
PPsat::conflict_analysis_dpll::post_backtrack(solver& solver, literal literal)
{
    const auto level_literal = literal.level_get();

    auto& [_, level_last] = consecutive_second_decisions.back();

    if (level_literal == level_last + 1)
    {
        ++level_last;
    }
    else
    {
        consecutive_second_decisions.emplace_back(level_literal - 1,
                                                  level_literal);
    }

    return solver.decide(!literal);
}

void PPsat::conflict_analysis_dpll::restart()
{}
