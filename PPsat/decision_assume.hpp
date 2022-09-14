#pragma once
#include "PPsat-base/literal.hpp"
#include "PPsat/assumptions.hpp"
#include <PPsat/decision.hpp>

#include <map>
#include <set>

namespace PPsat
{
class decision_assume final : public decision
{
    std::map<PPsat_base::variable*, bool> all;
    std::map<PPsat_base::variable*, bool> available;

public:
    decision_assume(assumptions& assumption);

    void assigned(PPsat_base::variable& variable) override final;
    void unassigned(PPsat_base::variable& variable) override final;
    PPsat_base::optional<PPsat_base::literal> get_decision() override final;
    void clause_learnt(const PPsat_base::clause& clause) override final;
    void conflict() override final;
};
}
