#include "PPsat/formula.hpp"
#include "PPsat/literal.hpp"
#include "PPsat/tseitin_builder.hpp"
#include "PPsat/variable.hpp"
#include <PPsat/tseitin_builder_nnf.hpp>

template <bool NNF>
PPsat::tseitin_builder_nnf<NNF>::tseitin_builder_nnf(
    class formula& formula) noexcept
    : formula(formula)
{}

namespace
{
PPsat::literal create_new_literal(const PPsat::tseitin_builder& builder)
{
    return {builder.create_new_variable(), true};
}
}

template <bool NNF>
PPsat::literal PPsat::tseitin_builder_nnf<NNF>::push_conjunction(
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
PPsat::literal PPsat::tseitin_builder_nnf<NNF>::push_disjunction(
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
PPsat::literal PPsat::tseitin_builder_nnf<NNF>::push_negation(
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
PPsat::literal PPsat::tseitin_builder_nnf<NNF>::push_literal() const
{
    auto introduced = create_new_literal(*this);

    formula.add_clause(introduced);

    return introduced;
}

template <bool NNF>
PPsat::variable& PPsat::tseitin_builder_nnf<NNF>::create_new_variable() const
{
    return formula.create_new_variable();
}

template class PPsat::tseitin_builder_nnf<true>;
template class PPsat::tseitin_builder_nnf<false>;
