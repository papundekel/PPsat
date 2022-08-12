#include <PPsat/formula.hpp>

#include <utility>

PPsat::formula::formula(factory_clause& clauses,
                        factory_variable& variables) noexcept
    : clauses(clauses)
    , variables(variables)
{}

void PPsat::formula::add_clause(view_any<const literal> literals)
{
    auto& new_clause = clauses.create(literals);

    for (const literal& literal : literals)
    {
        literal.register_containing_clause(new_clause);
    }
}

PPsat::variable& PPsat::formula::create_new_variable()
{
    return variables.create();
}

bool PPsat::formula::for_each(std::function<bool(clause&)> f)
{
    return clauses.for_each(std::move(f));
}

bool PPsat::formula::for_each(std::function<bool(const clause&)> f) const
{
    return std::as_const(clauses).for_each(std::move(f));
}

void PPsat::formula::for_each(std::function<void(variable&)> f)
{
    variables.for_each(
        [&f](variable& variable)
        {
            f(variable);
            return true;
        });
}

std::size_t PPsat::formula::clause_count() const noexcept
{
    return clauses.count();
}

void PPsat::formula::write_DIMACS(
    std::ostream& output,
    std::function<std::ostream&(std::ostream&, literal)> transform) const
{
    for_each(
        [&output, &transform](const clause& clause)
        {
            clause.for_each(
                [&output, &transform](literal literal)
                {
                    transform(output, literal) << " ";
                });
            output << "0\n";

            return true;
        });
}

bool PPsat::formula::contains_unsat_clause() const noexcept
{
    return for_each(
        [](const clause& clause)
        {
            if (clause.get_category_and_first_literal().first ==
                clause::category::unsat)
            {
                return false;
            }

            return true;
        });
}

std::optional<PPsat::literal> PPsat::formula::find_unit() const noexcept
{
    literal literal;

    const auto exists = for_each(
        [&literal](const clause& clause)
        {
            clause::category category;
            std::tie(category, literal) =
                clause.get_category_and_first_literal();
            if (category == clause::category::unit)
            {
                return false;
            }

            return true;
        });

    if (!exists)
    {
        return {};
    }

    return literal;
}
