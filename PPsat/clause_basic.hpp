#pragma once
#include <PPsat/clause.hpp>
#include <PPsat/literal.hpp>

#include <PPsat-base/view_any.hpp>
#include <PPsat-base/virtual_base.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

namespace PPsat
{
class clause_basic final : public clause
{
    std::vector<literal> literals;

public:
    clause_basic(PPsat_base::view_any<literal> literals)
        : literals(literals.begin(), literals.end())
    {
        reset();
    }

    void reset() override final
    {}

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

        bool found_sat = false;
        bool found_unassigned = false;
        bool found_unassigned_multiple = false;
        std::optional<literal> literal_unit_opt;

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
                if (found_unassigned)
                {
                    found_unassigned_multiple = true;
                }
                found_unassigned = true;
                literal_unit_opt.emplace(literal);
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

        if (found_unassigned_multiple)
        {
            return {false, false, {}};
        }

        return {false, false, *literal_unit_opt};
    }

    void unassign(literal literal_unassigned,
                  bool positive_in_clause) override final
    {}

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
