#pragma once
#include "PPsat/cli/parameters.hpp"
#include <PPsat/decision.hpp>

#include <random>
#include <unordered_map>
#include <vector>

namespace PPsat
{
class formula;

class decision_random final : public decision
{
    std::mt19937 generator;
    std::vector<variable*> set;

public:
    decision_random(formula& formula, const cli::parameters_value& parameters);

    void assigned(variable& variable) override final;
    void unassigned(variable& variable) override final;
    PPsat_base::optional<literal> get_decision() override final;
    void clause_learnt(const clause& clause) override final;
    void conflict() override final;
};
}
