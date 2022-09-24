#pragma once
#include <PPsat/literal.hpp>
#include <compare>

namespace PPsat
{
class variable;
class clause;
class formula;

class decision
{
public:
    enum class type
    {
        deterministic,
        trivial,
        random,
        JW_static,
        VSIDS,
    };

    virtual void reset(formula& formula) = 0;

    virtual void assigned(variable& variable) = 0;
    virtual void unassigned(variable& variable) = 0;
    virtual PPsat_base::optional<literal> get_decision() = 0;
    virtual void clause_learnt(const clause& clause) = 0;
    virtual void conflict() = 0;

    virtual ~decision() = default;
};

constexpr std::strong_ordering operator<=>(const decision::type& a,
                                           const decision::type& b)
{
    return (int)a <=> (int)b;
}
}
