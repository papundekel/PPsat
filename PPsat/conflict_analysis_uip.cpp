#include <PPsat/conflict_analysis_uip.hpp>

#include <PPsat/clause.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/literal.hpp>

#include <PPsat-base/ranges_to.hpp>
#include <PPsat-base/view_any.hpp>

#include <algorithm>
#include <list>
#include <ranges>
#include <utility>

#ifndef NDEBUG
#include <iostream>
#endif

namespace
{
void resolve(std::list<PPsat::literal>& C, const PPsat::clause& antecedent)
{
    antecedent.for_each(
        [&C](PPsat::literal literal)
        {
            const auto same_var = std::ranges::find_if(
                C,
                [literal](PPsat::literal literal_C)
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
    const PPsat_base::factory<formula::factory_clause>& clause_factory_factory,
    decision& decision) noexcept
    : decision_(decision)
    , learnt(clause_factory_factory.create())
{
    reset();
}

void PPsat::conflict_analysis_uip::reset()
{
    learnt->clear();
    unit_.reset();
    count = 0;
}

std::list<PPsat::unit> PPsat::conflict_analysis_uip::find_unary_unit() const
{
    std::list<unit> units;

    learnt->for_each(
        [&units](clause& clause)
        {
            for (const auto literal : clause.is_unary_unit())
            {
                units.emplace_back(literal, clause);
            }

            return true;
        });

    return units;
}

PPsat_base::optional<std::size_t> PPsat::conflict_analysis_uip::analyse(
    std::size_t level,
    const clause& antecedent)
{
    if (level == 0)
    {
        return {};
    }

    std::list<literal> C;
    antecedent.for_each(
        [&C](literal literal)
        {
            C.emplace_back(literal);
        });

    while (
        [&C, level]()
        {
            std::size_t count = 0;
            for (literal literal : C)
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
        const auto literal_ = *std::ranges::max_element(
            C,
            [](literal a, literal b)
            {
                return a.recency_get() < b.recency_get();
            });

        resolve(C, *literal_.antecedent_get());
    }

#ifndef NDEBUG
    std::cout << "L ";
    for (const auto literal : C)
    {
        std::cout << literal << " ";
    }
    std::cout << std::endl;
#endif

    std::optional<literal> literal_unit;
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

    auto& clause = learnt->create(PPsat_base::view_any<literal>(C));
    for (const auto literal : C)
    {
        if (clause.is_relevant(literal))
        {
            literal.register_(clause);
        }
    }

    decision_.clause_learnt(clause);

    unit_.emplace(*literal_unit, clause);

    return level_new;
}

std::pair<PPsat_base::optional<const PPsat::clause&>, std::list<PPsat::unit>>
PPsat::conflict_analysis_uip::post_backtrack(solver&, literal)
{
    std::list<unit> units;
    units.emplace_back(*unit_);
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
        [this, max_deleted, &deleted](clause& clause)
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
