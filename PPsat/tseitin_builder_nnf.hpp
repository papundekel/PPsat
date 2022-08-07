#pragma once
#include <PPsat/factory.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/literal_negated.hpp>
#include <PPsat/tseitin_builder.hpp>

namespace PPsat
{
template <bool NNF>
class tseitin_builder_nnf : public tseitin_builder
{
    class formula& formula;

public:
    tseitin_builder_nnf(class formula& formula) noexcept
        : formula(formula)
    {}

    void push_conjunction(const literal& p,
                          const literal& q,
                          const literal& r) const override final
    {
        if constexpr (!NNF)
        {
            formula.add_clause(p, !q, !r);
        }

        formula.add_clause(!p, q);
        formula.add_clause(!p, r);
    }

    void push_disjunction(const literal& p,
                          const literal& q,
                          const literal& r) const override final
    {
        formula.add_clause(!p, q, r);

        if constexpr (!NNF)
        {
            formula.add_clause(p, !q);
            formula.add_clause(p, !r);
        }
    }

    void push_negation(const literal& p, const literal& q) const override final
    {
        if constexpr (!NNF)
        {
            formula.add_clause(p, q);
        }

        formula.add_clause(!p, !q);
    }

    void push_literal(const literal& l) const override final
    {
        formula.add_clause(l);
    }
};
}
