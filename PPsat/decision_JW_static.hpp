#pragma once
#include "PPsat/cli/parameters.hpp"
#include <PPsat/decision.hpp>

#include <queue>
#include <random>
#include <unordered_map>
#include <vector>

namespace PPsat
{
class formula;

class decision_JW_static final : public decision
{
    class comparer
    {
    public:
        bool operator()(variable* a, variable* b) const;
    };

    std::priority_queue<variable*, std::vector<variable*>, comparer> set;

public:
    decision_JW_static(formula& formula,
                       const cli::parameters_value& parameters);

    void assigned(variable& variable) override final;
    void unassigned(variable& variable) override final;
    PPsat_base::optional<literal> get_decision() override final;
    void clause_learnt(const clause& clause) override final;
    void conflict() override final;
};
}
