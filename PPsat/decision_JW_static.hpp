#pragma once
#include "PPsat-base/variable.hpp"
#include <PPsat/decision.hpp>

#include <queue>
#include <random>
#include <unordered_map>
#include <vector>

namespace PPsat
{
class decision_JW_static final : public decision
{
    class comparer
    {
    public:
        bool operator()(PPsat_base::variable* a, PPsat_base::variable* b) const;
    };

    std::priority_queue<PPsat_base::variable*,
                        std::vector<PPsat_base::variable*>,
                        comparer>
        set;

public:
    decision_JW_static(PPsat_base::formula& formula);

private:
    void assigned(PPsat_base::variable& variable) override final;
    void unassigned(PPsat_base::variable& variable) override final;
    PPsat_base::optional<PPsat_base::literal> get_decision() override final;
    void clause_learnt(const PPsat_base::clause& clause) override final;
    void conflict() override final;
};
}
