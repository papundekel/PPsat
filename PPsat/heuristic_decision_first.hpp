#pragma once
#include <PPsat/heuristic_decision.hpp>

#include <set>

namespace PPsat
{
class heuristic_decision_first final : public heuristic_decision
{
    std::set<PPsat_base::variable*> set;

public:
    heuristic_decision_first(PPsat_base::formula& formula);

private:
    void assigned(PPsat_base::variable& variable) override final;
    void unassigned(PPsat_base::variable& variable) override final;

    PPsat_base::optional<PPsat_base::literal> get_decision()
        const override final;
};
}
