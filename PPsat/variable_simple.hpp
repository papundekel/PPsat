#pragma once
#include <PPsat-base/variable.hpp>

#include <string>
#include <variant>

namespace PPsat
{
class variable_simple : public virtual PPsat_base::variable
{
    void set_assignment(assignment assignment) override final;
    assignment get_assignment() const override final;

    void for_each_clause_containing(
        std::function<void(PPsat_base::clause&, bool)> f) const override final;

    void register_containing_clause(PPsat_base::clause& clause,
                                    bool positive) override final;
};
}
