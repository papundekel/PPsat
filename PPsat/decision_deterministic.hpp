#pragma once
#include <PPsat/cli/parameters.hpp>
#include <PPsat/decision.hpp>

#include <set>
#include <vector>

namespace PPsat
{
class formula;

class decision_deterministic final : public decision
{
    class comparer
    {
    public:
        bool operator()(const variable* a, const variable* b) const;
    };

    std::set<variable*, comparer> set;

public:
    decision_deterministic(formula& formula,
                           const cli::parameters_value& parameters);

    void reset(formula& formula) override final;

    void assigned(variable& variable) override final;
    void unassigned(variable& variable) override final;
    PPsat_base::optional<literal> get_decision() override final;
    void clause_learnt(const clause& clause) override final;
    void conflict() override final;
};
}
