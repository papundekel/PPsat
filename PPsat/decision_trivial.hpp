#pragma once
#include <PPsat/decision.hpp>

#include <unordered_set>
#include <vector>

namespace PPsat
{
class decision_trivial final : public decision
{
    std::unordered_set<PPsat_base::variable*> set;

public:
    decision_trivial(PPsat_base::formula& formula);

private:
    void assigned(PPsat_base::variable& variable) override final;
    void unassigned(PPsat_base::variable& variable) override final;
    PPsat_base::optional<PPsat_base::literal> get_decision() override final;
    void clause_learnt(const PPsat_base::clause& clause) override final;
    void conflict() override final;
};
}
