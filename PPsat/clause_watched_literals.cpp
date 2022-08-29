#include "PPsat-base/literal.hpp"
#include "PPsat-base/ranges_to.hpp"
#include "PPsat-base/variable.hpp"
#include "PPsat-base/variable_assignment.hpp"
#include "PPsat/variable_assignable.hpp"
#include <PPsat/clause_watched_literals.hpp>

#include <algorithm>
#include <cassert>
#include <ranges>
#include <tuple>

PPsat::clause_watched_literals::clause_watched_literals(
    PPsat_base::view_any<PPsat_base::literal> literals)
    : literals(literals.begin(), literals.end())
    , watched1(this->literals.front())
    , watched2(watched1)
{
    if (this->literals.size() != 1)
    {
        auto recent_first = this->literals.front();
        auto recent_second = *(this->literals.begin() + 1);
        auto recency_first = recent_first.recency_get();
        auto recency_second = recent_second.recency_get();

        if (recency_second > recency_first)
        {
            std::swap(recent_first, recent_second);
            std::swap(recency_first, recency_second);
        }

        for (auto i = 2uz; i != this->literals.size(); ++i)
        {
            const auto literal = this->literals[i];
            const auto recency = literal.recency_get();
            if (recency > recency_first)
            {
                recent_second = recent_first;
                recency_second = recency_first;
                recent_first = literal;
                recency_first = recency;
            }
            else if (recency > recency_second)
            {
                recent_second = literal;
                recency_second = recency;
            }
        }

        watched1 = recent_first;
        watched2 = recent_second;
        assert(watched1 != watched2);
    }
}

void PPsat::clause_watched_literals::for_each(
    std::function<void(PPsat_base::literal)> f) const
{
    std::ranges::for_each(literals, f);
}

PPsat_base::optional<PPsat_base::literal>
PPsat::clause_watched_literals::is_unary_unit() const
{
    if (literals.size() != 1)
    {
        return {};
    }

    return literals | std::views::filter(
                          [](const PPsat_base::literal literal)
                          {
                              return literal.assignment_get() ==
                                     PPsat_base::variable_assignment::unknown;
                          });
}

std::pair<bool, std::optional<PPsat_base::literal>>
PPsat::clause_watched_literals::assign(PPsat_base::literal literal_assigned,
                                       bool positive_in_clause)
{
    const PPsat_base::literal literal_in_clause(literal_assigned,
                                                positive_in_clause);

    if (literal_assigned.is_positive() == literal_in_clause.is_positive())
    {
        return {false, {}};
    }

    if (literal_in_clause == watched2)
    {
        std::swap(watched1, watched2);
    }

    bool found_sat = false;
    bool found_unassigned = false;
    std::optional<PPsat_base::literal> other_opt;

    for (const auto literal : literals)
    {
        const auto assignment = literal.assignment_get();
        if (assignment == PPsat_base::variable_assignment::positive)
        {
            found_sat = true;
            break;
        }
        else if (assignment == PPsat_base::variable_assignment::unknown)
        {
            found_unassigned = true;

            if (literal != watched2)
            {
                other_opt.emplace(literal);
            }
        }
    }

    if (found_sat)
    {
        return {false, {}};
    }

    if (!found_unassigned)
    {
        return {true, {}};
    }

    if (other_opt)
    {
        watched1.unregister(*this);
        watched1 = *other_opt;
        watched1.register_(*this);

        return {false, {}};
    }

    return {false, watched2};
}

void PPsat::clause_watched_literals::unassign(PPsat_base::literal, bool)
{}

bool PPsat::clause_watched_literals::is_relevant(
    PPsat_base::literal literal) const
{
    return literal == watched1 || literal == watched2;
}

bool PPsat::clause_watched_literals::antecedent_to_some() const
{
    if (watched1.assignment_get() == PPsat_base::variable_assignment::positive)
    {
        for (const auto& antecedent : watched1.antecedent_get())
        {
            if (&antecedent == this)
            {
                return true;
            }
        }
    }

    if (watched2.assignment_get() == PPsat_base::variable_assignment::positive)
    {
        for (const auto& antecedent : watched2.antecedent_get())
        {
            if (&antecedent == this)
            {
                return true;
            }
        }
    }

    return false;
}

void PPsat::clause_watched_literals::unregister()
{
    watched1.unregister(*this);
    watched2.unregister(*this);
}

std::size_t PPsat::clause_watched_literals::length() const
{
    return literals.size();
}