#pragma once
#include <PPsat/heuristic_decision.hpp>

#include <map>
#include <memory>

namespace PPsat_base
{
class variable;
class formula;
}

namespace PPsat
{
class heuristic_decision_priority final : public heuristic_decision
{
    std::unique_ptr<heuristic_decision> priority;
    std::unique_ptr<heuristic_decision> fallback;

public:
    heuristic_decision_priority(std::unique_ptr<heuristic_decision> priority,
                                std::unique_ptr<heuristic_decision> fallback);

private:
    void assigned(PPsat_base::variable& variable) override final;
    void unassigned(PPsat_base::variable& variable) override final;

    PPsat_base::optional<PPsat_base::literal> get_decision()
        const override final;
};
}
