#pragma once
#include <PPsat/formula.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/tseitin_builder.hpp>

#include <PPsat-base/factory.hpp>

namespace PPsat
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

extern template class PPsat::tseitin_builder_nnf<true>;
extern template class PPsat::tseitin_builder_nnf<false>;
