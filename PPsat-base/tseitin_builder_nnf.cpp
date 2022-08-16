#include <PPsat-base/formula.hpp>
#include <PPsat-base/literal.hpp>
#include <PPsat-base/tseitin_builder.hpp>
#include <PPsat-base/tseitin_builder_nnf.hpp>
#include <PPsat-base/variable.hpp>

template <bool NNF>
PPsat_base::tseitin_builder_nnf<NNF>::tseitin_builder_nnf(
    class formula& formula) noexcept
    : formula(formula)
{}

namespace
{
PPsat_base::literal create_new_literal(
    const PPsat_base::tseitin_builder& builder)
{
    return {builder.create_new_variable(), true};
}
}

template <bool NNF>
PPsat_base::literal PPsat_base::tseitin_builder_nnf<NNF>::push_conjunction(
    literal left,
    literal right) const
{
    auto introduced = create_new_literal(*this);

    if constexpr (!NNF)
    {
        formula.add_clause(introduced, !left, !right);
    }

    formula.add_clause(!introduced, left);
    formula.add_clause(!introduced, right);

    return introduced;
}

template <bool NNF>
PPsat_base::literal PPsat_base::tseitin_builder_nnf<NNF>::push_disjunction(
    literal left,
    literal right) const
{
    auto introduced = create_new_literal(*this);

    formula.add_clause(!introduced, left, right);

    if constexpr (!NNF)
    {
        formula.add_clause(introduced, !left);
        formula.add_clause(introduced, !right);
    }

    return introduced;
}

template <bool NNF>
PPsat_base::literal PPsat_base::tseitin_builder_nnf<NNF>::push_negation(
    literal inner) const
{
    auto introduced = create_new_literal(*this);

    if constexpr (!NNF)
    {
        formula.add_clause(introduced, inner);
    }

    formula.add_clause(!introduced, !inner);

    return introduced;
}

template <bool NNF>
void PPsat_base::tseitin_builder_nnf<NNF>::push_literal(literal literal) const
{
    formula.add_clause(literal);
}

template <bool NNF>
PPsat_base::variable&
PPsat_base::tseitin_builder_nnf<NNF>::create_new_variable() const
{
    return formula.create_new_variable();
}

template class PPsat_base::tseitin_builder_nnf<true>;
template class PPsat_base::tseitin_builder_nnf<false>;
