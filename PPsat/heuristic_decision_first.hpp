#pragma once
#include <PPsat/heuristic_decision.hpp>

#include <set>

namespace PPsat
{
class variable;

class heuristic_decision_first final : public heuristic_decision
{
    std::set<variable*> set;

    void init(formula& formula) override final;

    void assigned(variable& variable) override final;
    void unassigned(variable& variable) override final;

    variable& get_decision() const override final;
};
}
