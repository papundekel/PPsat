#pragma once
#include <PPsat/heuristic_decision.hpp>

#include <set>

namespace PPsat
{
class heuristic_decision_random final : public heuristic_decision
{
    std::vector<PPsat_base::variable*> set;

public:
    heuristic_decision_random(PPsat_base::formula& formula);

private:
    void assigned(PPsat_base::variable& variable) override final;
    void unassigned(PPsat_base::variable& variable) override final;

    PPsat_base::optional<PPsat_base::literal> get_decision()
        const override final;
};
}
