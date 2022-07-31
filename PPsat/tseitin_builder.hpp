#pragma once
#include <PPsat/literal.hpp>

namespace PPsat
{
class tseitin_builder
{
public:
    virtual void push_conjunction(const literal& p, const literal& q, const literal& r) const = 0;
    virtual void push_disjunction(const literal& p, const literal& q, const literal& r) const = 0;
    virtual void push_negation(const literal& p, const literal& q) const = 0;
    virtual void push_literal(const literal& l) const = 0;

    virtual ~tseitin_builder() {}
};
}
