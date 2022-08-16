#pragma once
#include <PPsat-base/factory.hpp>
#include <PPsat-base/formula.hpp>
#include <PPsat-base/literal.hpp>
#include <PPsat-base/tseitin_builder.hpp>

namespace PPsat_base
{
template <bool NNF>
class tseitin_builder_nnf final : public tseitin_builder
{
    class formula& formula;

public:
    tseitin_builder_nnf(class formula& formula) noexcept;

    literal push_conjunction(literal left, literal right) const override final;
    literal push_disjunction(literal left, literal right) const override final;
    literal push_negation(literal inner) const override final;
    void push_literal(literal literal) const override final;
    variable& create_new_variable() const override final;
};
}

extern template class PPsat_base::tseitin_builder_nnf<true>;
extern template class PPsat_base::tseitin_builder_nnf<false>;
