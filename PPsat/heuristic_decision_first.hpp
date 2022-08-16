#pragma once
#include <PPsat/heuristic_decision.hpp>

#include <set>

namespace PPsat_base
{
class variable;
class formula;
}

namespace PPsat
{
class heuristic_decision_first final : public heuristic_decision
{
    std::set<PPsat_base::variable*> set;

    void init(PPsat_base::formula& formula) override final;

    void assigned(PPsat_base::variable& variable) override final;
    void unassigned(PPsat_base::variable& variable) override final;

    PPsat_base::variable& get_decision() const override final;
};
}
