#pragma once
#include <PPsat-base/literal.hpp>
#include <PPsat-base/variable.hpp>

namespace PPsat_base
{
class tseitin_builder
{
public:
    virtual literal push_conjunction(literal left, literal right) const = 0;
    virtual literal push_disjunction(literal left, literal right) const = 0;
    virtual literal push_negation(literal inner) const = 0;
    virtual void push_literal(literal literal) const = 0;
    virtual variable& create_new_variable() const = 0;

    virtual ~tseitin_builder() = default;
};
}
