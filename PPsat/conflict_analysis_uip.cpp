#include "PPsat-base/clause.hpp"
#include "PPsat-base/literal.hpp"
#include "PPsat-base/view_any.hpp"
#include <PPsat/conflict_analysis_uip.hpp>

#include <algorithm>
#include <list>
#include <utility>

#ifndef NDEBUG
#include <iostream>
#endif

namespace
{
void resolve(std::list<PPsat_base::literal>& C,
             const PPsat_base::clause& antecedent)
{
    antecedent.for_each(
        [&C](PPsat_base::literal literal)
        {
            const auto same_var = std::ranges::find_if(
                C,
                [literal](PPsat_base::literal literal_C)
                {
                    return &literal.get_variable() == &literal_C.get_variable();
                });

            if (same_var == C.end())
            {
                C.emplace_back(literal);
                return;
            }

            if (same_var->is_positive() == literal.is_positive())
            {
                return;
            }

            C.erase(same_var);
        });
}
}

PPsat::conflict_analysis_uip::conflict_analysis_uip(
    PPsat_base::unique_ref<PPsat_base::formula::factory_clause>&&
        learnt) noexcept
    : learnt(std::move(learnt))
    , count(0)
{}

bool PPsat::conflict_analysis_uip::for_each(
    std::function<bool(const PPsat_base::clause&)> f) const
{
    return std::as_const(*learnt.get()).for_each(f);
}

PPsat_base::optional<std::size_t> PPsat::conflict_analysis_uip::analyse(
    std::size_t level,
    const PPsat_base::clause& antecedent)
{
    if (level == 0)
    {
        return {};
    }

    std::list<PPsat_base::literal> C;
    antecedent.for_each(
        [&C](PPsat_base::literal literal)
        {
            C.emplace_back(literal);
        });

    while (
        [&C, level]()
        {
            std::size_t count = 0;
            for (PPsat_base::literal literal : C)
            {
                if (literal.level_get() == level)
                {
                    ++count;
                    if (count > 1)
                    {
                        return true;
                    }
                }
            }

            return false;
        }())
    {
        const auto literal = *std::ranges::max_element(
            C,
            [](PPsat_base::literal a, PPsat_base::literal b)
            {
                return a.recency_get() < b.recency_get();
            });

        resolve(C, literal.antecedent_get());
    }

#ifndef NDEBUG
    std::cout << "L ";
    for (const auto literal : C)
    {
        std::cout << literal << " ";
    }
    std::cout << std::endl;
#endif

    std::optional<PPsat_base::literal> literal_unit;
    std::size_t level_new = 0;
    for (const auto literal : C)
    {
        const auto level_C = literal.level_get();

        if (level_C == level)
        {
            literal_unit = literal;
        }
        else if (level_C > level_new)
        {
            level_new = level_C;
        }
    }

    auto& clause = learnt->create(PPsat_base::view_any<PPsat_base::literal>(C));
    for (const auto literal : C)
    {
        if (clause.is_relevant(literal))
        {
            literal.register_(clause);
        }
    }

    unit.emplace(*literal_unit, clause);

    return level_new;
}

std::pair<PPsat_base::optional<const PPsat_base::clause&>,
          std::list<PPsat_base::unit>>
PPsat::conflict_analysis_uip::post_backtrack(solver&, PPsat_base::literal)
{
    std::list<PPsat_base::unit> units;
    units.emplace_back(*unit);
    return {{}, std::move(units)};
}

void PPsat::conflict_analysis_uip::restarted()
{}
