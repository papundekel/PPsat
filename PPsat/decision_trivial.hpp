#pragma once
#include <PPsat/cli/parameters.hpp>
#include <PPsat/decision.hpp>

#include <unordered_set>
#include <vector>

namespace PPsat
{
class formula;

class decision_trivial final : public decision
{
    std::unordered_set<variable*> set;

public:
    decision_trivial(formula& formula, const cli::parameters_value& parameters);

    void reset(formula& formula) override final;

    void assigned(variable& variable) override final;
    void unassigned(variable& variable) override final;
    PPsat_base::optional<literal> get_decision() override final;
    void clause_learnt(const clause& clause) override final;
    void conflict() override final;
};
}
