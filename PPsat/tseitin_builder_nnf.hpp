#pragma once
#include <PPsat/formula.hpp>
#include <PPsat/tseitin_builder.hpp>

namespace PPsat
{
template <bool NNF>
class tseitin_builder_nnf : public tseitin_builder
{
    formula& builder;

public:
    tseitin_builder_nnf(formula& builder) noexcept
        : builder(builder)
    {}

    void push_conjunction(const literal& p,
                          const literal& q,
                          const literal& r) const override final
    {
        if constexpr (!NNF)
        {
            builder.add_clause(p, !q, !r);
        }

        builder.add_clause(!p, q);
        builder.add_clause(!p, r);
    }
    
    void push_disjunction(const literal& p,
                          const literal& q,
                          const literal& r) const override final
    {
        builder.add_clause(!p, q, r);

        if constexpr (!NNF)
        {
            builder.add_clause(p, !q);
            builder.add_clause(p, !r);
        }
    }

    void push_negation(const literal& p, const literal& q) const override final
    {
        if constexpr (!NNF)
        {
            builder.add_clause(p, q);
        }

        builder.add_clause(!p, !q);
    }

    void push_literal(const literal& l) const override final
    {
        builder.add_clause(l);
    }
};
}
