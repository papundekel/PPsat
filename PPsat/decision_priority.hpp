#pragma once
#include <PPsat/decision.hpp>

#include <map>
#include <memory>

namespace PPsat_base
{
class variable;
class formula;
}

namespace PPsat
{
class decision_priority final : public decision
{
    std::unique_ptr<decision> priority;
    std::unique_ptr<decision> fallback;

public:
    decision_priority(std::unique_ptr<decision> priority,
                      std::unique_ptr<decision> fallback);

    void reset(formula& formula) override final;

    void assigned(variable& variable) override final;
    void unassigned(variable& variable) override final;
    PPsat_base::optional<literal> get_decision() override final;
    void clause_learnt(const clause& clause) override final;
    void conflict() override final;
};
}
