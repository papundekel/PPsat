#include <PPsat-base/formula.hpp>

#include <utility>

PPsat_base::formula::formula(factory_clause& clauses,
                             factory_variable& variables) noexcept
    : clauses(clauses)
    , variables(variables)
{}

PPsat_base::clause& PPsat_base::formula::add_clause(
    view_any<const literal> literals)
{
    auto& clause_new = clauses.create(literals);

    for (const literal& literal : literals)
    {
        if (clause_new.is_relevant(literal))
        {
            literal.register_(clause_new);
        }
    }

    return clause_new;
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
                [&output, &transform](literal literal)
                {
                    transform(output, literal) << " ";
                });
            output << "0\n";

            return true;
        });
}

std::optional<PPsat_base::literal> PPsat_base::formula::find_unit()
    const noexcept
{
    std::optional<literal> literal_opt;

    const auto exists = for_each(
        [&literal_opt](const clause& clause)
        {
            literal_opt = clause.is_unit();
            return !literal_opt.has_value();
        });

    if (!exists)
    {
        return {};
    }

    return literal_opt;
}
