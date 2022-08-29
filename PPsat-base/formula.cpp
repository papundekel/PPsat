#include "PPsat-base/clause.hpp"
#include "PPsat-base/optional.hpp"
#include <PPsat-base/formula.hpp>

#include <algorithm>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

PPsat_base::formula::formula(const preprocessor& preprocessor,
                             factory_clause& clauses,
                             factory_variable& variables) noexcept
    : preprocessor_(preprocessor)
    , clauses(clauses)
    , variables(variables)
    , empty_clause(false)
{}

void PPsat_base::formula::add_clause(view_any<literal> literals)
{
    auto [literals_preprocessed_opt, empty] =
        preprocessor_.preprocess(literals);

    if (empty)
    {
        empty_clause = true;
    }

    if (!literals_preprocessed_opt)
    {
        return;
    }

    auto literals_preprocessed = std::move(*literals_preprocessed_opt);

    auto& clause = clauses.create(view_any<literal>(literals_preprocessed));

    for (const auto literal : literals_preprocessed)
    {
        if (clause.is_relevant(literal))
        {
            literal.register_(clause);
        }
    }
}

PPsat_base::variable& PPsat_base::formula::create_new_variable()
{
    return variables.create();
}

bool PPsat_base::formula::for_each(std::function<bool(clause&)> f)
{
    return clauses.for_each(std::move(f));
}

bool PPsat_base::formula::for_each(std::function<bool(const clause&)> f) const
{
    return std::as_const(clauses).for_each(std::move(f));
}

void PPsat_base::formula::for_each(std::function<void(variable&)> f)
{
    variables.for_each(
        [&f](variable& variable)
        {
            f(variable);
            return true;
        });
}
std::size_t PPsat_base::formula::count_clause() const noexcept
{
    return clauses.count();
}
std::size_t PPsat_base::formula::count_variable() const noexcept
{
    return variables.count();
}

void PPsat_base::formula::write_DIMACS(
    std::ostream& output,
    std::function<std::ostream&(std::ostream&, literal)> transform) const
{
    for_each(
        [&output, &transform](const clause& clause)
        {
            clause.for_each(
                [&output, &transform](const literal literal)
                {
                    transform(output, literal) << " ";
                });
            output << "0\n";

            return true;
        });
}

bool PPsat_base::formula::has_empty_clause()
{
    return empty_clause;
}

std::list<PPsat_base::unit> PPsat_base::formula::find_unary_unit() const
{
    std::list<PPsat_base::unit> units;

    for_each(
        [&units](const clause& clause)
        {
            const auto literal_opt = clause.is_unary_unit();

            if (literal_opt)
            {
                units.emplace_back(*literal_opt, clause);
            }

            return !literal_opt;
        });

    return units;
}
