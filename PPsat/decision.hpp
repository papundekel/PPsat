#pragma once
#include "PPsat-base/clause.hpp"
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
class decision
{
public:
    virtual void assigned(PPsat_base::variable& variable) = 0;
    virtual void unassigned(PPsat_base::variable& variable) = 0;
    virtual PPsat_base::optional<PPsat_base::literal> get_decision() = 0;
    virtual void clause_learnt(const PPsat_base::clause& clause) = 0;
    virtual void conflict() = 0;

    virtual ~decision() = default;
};
}
