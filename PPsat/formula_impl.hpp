#pragma once
#include "PPsat/cli/parameters.hpp"
#include "PPsat/variable.hpp"
#include <PPsat/formula.hpp>

#include <functional>
#include <ranges>
#include <type_traits>

namespace PPsat
{
template <typename FactoryClause, typename FactoryVariable>
class formula_impl final : public formula
{
private:
    const preprocessor& preprocessor_;
    FactoryClause& clauses;
    FactoryVariable& variables;
    bool empty_clause;

public:
    formula_impl(const preprocessor& preprocessor,
                 FactoryClause& clauses,
                 FactoryVariable& variables) noexcept
        : preprocessor_(preprocessor)
        , clauses(clauses)
        , variables(variables)
        , empty_clause(false)
    {}

    std::size_t count_clause() const noexcept override final
    {
        return clauses.count();
    }
    std::size_t count_variable() const noexcept override final
    {
        return variables.count();
    }

    variable& create_new_variable() override final
    {
        return variables.create();
    }

    void add_clause(PPsat_base::view_any<literal> literals) override final
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

        auto& clause = clauses.create(
            PPsat_base::view_any<literal>(literals_preprocessed));

        for (const auto literal : literals_preprocessed)
        {
            if (clause.is_relevant(literal))
            {
                literal.register_(clause);
            }
        }
    }
    void add_clause(auto... literals)
    {
        add_clause(PPsat_base::view_any<literal>(std::array{literals...}));
    }

    bool for_each_clause(std::function<bool(clause&)> f) override final
    {
        return clauses.for_each(std::forward<decltype(f)>(f));
    }

    bool for_each_clause(
        std::function<bool(const clause&)> f) const override final
    {
        return std::as_const(clauses).for_each(std::forward<decltype(f)>(f));
    }

    void for_each_variable(std::function<void(variable&)> f) override final
    {
        variables.for_each(
            [&f](variable& variable)
            {
                std::forward<decltype(f)>(f)(variable);
                return true;
            });
    }

    void write_DIMACS(std::ostream& output,
                      std::function<std::ostream&(std::ostream&, literal)>
                          transform = literal_printer) const override final
    {
        for_each_clause(
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

    std::list<unit> find_unary_unit() const override final
    {
        std::list<PPsat::unit> units;

        for_each_clause(
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

    bool has_empty_clause() override final
    {
        return empty_clause;
    }
};
}
