#pragma once
#include <PPsat/assumptions.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/literal.hpp>

#include <map>
#include <set>

namespace PPsat
{
class decision_assume final : public decision
{
    std::map<variable*, bool> all;
    std::map<variable*, bool> available;

public:
    decision_assume(formula& formula, assumptions& assumption);

    void reset(formula& formula) override final;

    void assigned(variable& variable) override final;
    void unassigned(variable& variable) override final;
    PPsat_base::optional<literal> get_decision() override final;
    void clause_learnt(const clause& clause) override final;
    void conflict() override final;
};
}
