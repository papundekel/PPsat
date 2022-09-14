#pragma once
#include <PPsat/decision.hpp>

#include <set>
#include <vector>

namespace PPsat
{
class decision_deterministic final : public decision
{
    class comparer
    {
    public:
        bool operator()(const PPsat_base::variable* a,
                        const PPsat_base::variable* b) const;
    };

    std::set<PPsat_base::variable*, comparer> set;

public:
    decision_deterministic(PPsat_base::formula& formula);

private:
    void assigned(PPsat_base::variable& variable) override final;
    void unassigned(PPsat_base::variable& variable) override final;
    PPsat_base::optional<PPsat_base::literal> get_decision() override final;
    void clause_learnt(const PPsat_base::clause& clause) override final;
    void conflict() override final;
};
}
