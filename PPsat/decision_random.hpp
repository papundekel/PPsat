#pragma once
#include <PPsat/decision.hpp>

#include <random>
#include <unordered_map>
#include <vector>

namespace PPsat
{
class decision_random final : public decision
{
    std::mt19937 generator;
    std::vector<PPsat_base::variable*> set;

public:
    decision_random(PPsat_base::formula& formula, std::size_t seed);

    void assigned(PPsat_base::variable& variable) override final;
    void unassigned(PPsat_base::variable& variable) override final;
    PPsat_base::optional<PPsat_base::literal> get_decision() override final;
    void clause_learnt(const PPsat_base::clause& clause) override final;
    void conflict() override final;
};
}
