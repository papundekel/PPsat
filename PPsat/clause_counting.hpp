#pragma once
#include <PPsat/clause.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/variable.hpp>

#include <PPsat-base/optional.hpp>
#include <PPsat-base/view_any.hpp>
#include <PPsat-base/virtual_base.hpp>

#include <algorithm>

namespace PPsat
{
class clause_counting final : public clause
{
    std::vector<literal> literals;
    std::size_t unassigned;
    std::size_t assigned_false;
    std::size_t assigned_true;

public:
    clause_counting(PPsat_base::view_any<literal> literals)
        : literals(literals.begin(), literals.end())
    {
        reset();
    }

    void reset() override final
    {
        unassigned = this->literals.size();
        assigned_false = 0;
        assigned_true = 0;
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
        --unassigned;

        if (literal_assigned.is_positive() == positive_in_clause)
        {
            ++assigned_true;
            return {false, false, {}};
        }

        ++assigned_false;

        if (assigned_true != 0)
        {
            return {false, false, {}};
        }

        if (unassigned == 0)
        {
            return {false, true, {}};
        }

        if (unassigned != 1)
        {
            return {false, false, {}};
        };

        const auto i =
            std::ranges::find_if(literals,
                                 [](literal literal)
                                 {
                                     return literal.assignment_get() ==
                                            variable::assignment::unknown;
                                 });

        return {false, false, *i};
    }

    void unassign(literal literal_unassigned,
                  bool positive_in_clause) override final
    {
        if (literal_unassigned.is_positive() == positive_in_clause)
        {
            --assigned_true;
        }
        else
        {
            --assigned_false;
        }

        ++unassigned;
    }

    bool is_relevant(literal literal) const override final
    {
        return true;
    }

    bool antecedent_to_some() const override final
    {
        if (literals.size() == 1)
        {
            return true;
        }

        if (assigned_true == 0)
        {
            return false;
        }

        const auto i = std::ranges::find_if(literals,
                                            [this](const literal literal)
                                            {
                                                for (const auto& antecedent :
                                                     literal.antecedent_get())
                                                {
                                                    return &antecedent == this;
                                                }

                                                return false;
                                            });

        return i != literals.end();
    }

    void unregister() override final
    {
        std::ranges::for_each(literals,
                              [this](const literal literal)
                              {
                                  literal.unregister(*this);
                              });
    }

    std::size_t length() const override final
    {
        return literals.size();
    }
};
}
