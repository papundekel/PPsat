#pragma once
#include "PPsat-base/literal.hpp"
#include "PPsat/assumptions.hpp"
#include <PPsat/heuristic_decision.hpp>

#include <map>
#include <set>

namespace PPsat
{
class heuristic_decision_assume final : public heuristic_decision
{
    std::map<PPsat_base::variable*, bool> all;
    std::map<PPsat_base::variable*, bool> available;

public:
    heuristic_decision_assume(assumptions& assumption);

private:
    void assigned(PPsat_base::variable& variable) override final;
    void unassigned(PPsat_base::variable& variable) override final;

    PPsat_base::optional<PPsat_base::literal> get_decision()
        const override final;
};
}
