#pragma once
#include "PPsat-base/variable.hpp"
#include <PPsat/assumptions.hpp>

#include <PPsat-base/literal.hpp>

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
    virtual void assigned(PPsat_base::variable& variable) = 0;
    virtual void unassigned(PPsat_base::variable& variable) = 0;
    virtual PPsat_base::optional<PPsat_base::literal> get_decision() const = 0;

    virtual ~heuristic_decision() = default;
};
}
