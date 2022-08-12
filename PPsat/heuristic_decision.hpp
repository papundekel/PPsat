#pragma once
#include "PPsat/variable.hpp"
#include <cstddef>

namespace PPsat
{
class formula;
class variable;

class heuristic_decision
{
public:
    virtual void init(formula& variables) = 0;
    virtual void assigned(variable& variable) = 0;
    virtual void unassigned(variable& variable) = 0;
    virtual variable& get_decision() const = 0;
};
}
