#pragma once
#include <PPsat/literal.hpp>

namespace PPsat
{
class variable;
class clause;

class decision
{
public:
    enum class type
    {
        trivial,
        deterministic,
        random,
        JW_static,
        VSIDS,
    };

    virtual void assigned(variable& variable) = 0;
    virtual void unassigned(variable& variable) = 0;
    virtual PPsat_base::optional<literal> get_decision() = 0;
    virtual void clause_learnt(const clause& clause) = 0;
    virtual void conflict() = 0;

    virtual ~decision() = default;
};
}
