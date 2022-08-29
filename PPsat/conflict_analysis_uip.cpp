#include "PPsat-base/clause.hpp"
#include "PPsat-base/literal.hpp"
#include "PPsat-base/ranges_to.hpp"
#include "PPsat-base/view_any.hpp"
#include "PPsat/decision.hpp"
#include <PPsat/conflict_analysis_uip.hpp>

#include <algorithm>
#include <bits/ranges_util.h>
#include <list>
#include <ranges>
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
    PPsat_base::unique_ref<PPsat_base::formula::factory_clause>&& learnt,
    decision& decision) noexcept
    : decision_(decision)
    , learnt(std::move(learnt))
    , learnt_unary()
    , unary_last_found(std::prev(learnt_unary.end()))
    , unit()
    , count(0)
{}

std::list<PPsat_base::unit> PPsat::conflict_analysis_uip::find_unary_unit()
    const
{
    std::list<PPsat_base::unit> units;

    for (PPsat_base::clause& clause :
         std::ranges::subrange(std::next(unary_last_found), learnt_unary.end()))
    {
        for (auto literal : clause.is_unary_unit())
        {
            units.emplace_back(literal, clause);
        }
    }

    unary_last_found = std::prev(learnt_unary.end());

    return units;
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

        resolve(C, *literal.antecedent_get());
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

    auto& clause =
        C.size() == 1
            ? learnt_unary.emplace_back(C.back())
            : learnt->create(PPsat_base::view_any<PPsat_base::literal>(C));
    for (const auto literal : C)
    {
        if (clause.is_relevant(literal))
        {
            literal.register_(clause);
        }
    }

    decision_.clause_learnt(clause);

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

void PPsat::conflict_analysis_uip::restart()
{
#ifndef NDEBUG
    std::cout << "LSB " << learnt->count() << std::endl;
#endif

    const auto max_deleted = learnt->count() / 2;
    auto deleted = 0uz;
    learnt->erase(
        [this, max_deleted, &deleted](PPsat_base::clause& clause)
        {
            if (deleted >= max_deleted)
            {
                return false;
            }

            if (clause.length() == 2)
            {
                return false;
            }

            if (clause.antecedent_to_some())
            {
                return false;
            }

            clause.unregister();

            ++deleted;
            return true;
        });

#ifndef NDEBUG
    std::cout << "LSA " << learnt->count() << std::endl;
#endif
}
