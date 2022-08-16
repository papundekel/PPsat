#pragma once
#include <cstddef>

namespace PPsat_base
{
class formula;
class variable;
}

namespace PPsat
{
class heuristic_decision
{
public:
    virtual void init(PPsat_base::formula& variables) = 0;
    virtual void assigned(PPsat_base::variable& variable) = 0;
    virtual void unassigned(PPsat_base::variable& variable) = 0;
    virtual PPsat_base::variable& get_decision() const = 0;
};
}
