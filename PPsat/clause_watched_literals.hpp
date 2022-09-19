#pragma once
#include <PPsat/clause.hpp>
#include <PPsat/literal.hpp>

#include <PPsat-base/view_any.hpp>
#include <PPsat-base/virtual_base.hpp>

#include <algorithm>
#include <cassert>

namespace PPsat
{
class clause_watched_literals final : public clause
{
    using literals_t = std::vector<literal>;

    literals_t literals;
    literal watched1;
    literal watched2;

public:
    clause_watched_literals(PPsat_base::view_any<literal> literals)
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

    void for_each(std::function<void(literal)> f) const override final
    {
        std::ranges::for_each(literals, f);
    }

    PPsat_base::optional<PPsat::literal> is_unary_unit() const override final
    {
        if (literals.size() != 1)
        {
            return {};
        }

        return literals | std::views::filter(
                              [](const literal literal)
                              {
                                  return literal.assignment_get() ==
                                         variable_assignment::unknown;
                              });
    }

    std::tuple<bool, bool, std::optional<PPsat::literal>> assign(
        literal literal_assigned,
        bool positive_in_clause) override final
    {
        const literal literal_in_clause(literal_assigned, positive_in_clause);

        if (literal_assigned.is_positive() == literal_in_clause.is_positive())
        {
            return {false, false, {}};
        }

        if (literal_in_clause == watched2)
        {
            std::swap(watched1, watched2);
        }

        bool found_sat = false;
        bool found_unassigned = false;
        std::optional<literal> other_opt;

        for (const auto literal : literals)
        {
            const auto assignment = literal.assignment_get();
            if (assignment == variable_assignment::positive)
            {
                found_sat = true;
                break;
            }
            else if (assignment == variable_assignment::unknown)
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
            return {false, false, {}};
        }

        if (!found_unassigned)
        {
            return {false, true, {}};
        }

        if (other_opt)
        {
            watched1 = *other_opt;
            watched1.register_(*this);

            return {true, false, {}};
        }

        return {false, false, watched2};
    }

    void unassign(literal, bool) override final
    {}

    bool is_relevant(literal literal) const override final
    {
        return literal == watched1 || literal == watched2;
    }

    bool antecedent_to_some() const override final
    {
        if (watched1.assignment_get() == variable_assignment::positive)
        {
            for (const auto& antecedent : watched1.antecedent_get())
            {
                if (&antecedent == this)
                {
                    return true;
                }
            }
        }

        if (watched2.assignment_get() == variable_assignment::positive)
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

    void unregister() override final
    {
        watched1.unregister(*this);
        watched2.unregister(*this);
    }

    std::size_t length() const override final
    {
        return literals.size();
    }
};
}
