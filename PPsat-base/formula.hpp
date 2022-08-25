#pragma once
#include "PPsat-base/optional.hpp"
#include <PPsat-base/clause.hpp>
#include <PPsat-base/container_factory.hpp>
#include <PPsat-base/literal.hpp>
#include <PPsat-base/preprocessor.hpp>
#include <PPsat-base/variable.hpp>
#include <PPsat-base/view_any.hpp>

#include <functional>
#include <ranges>

namespace PPsat_base
{
class formula
{
public:
    using factory_clause = container_factory<clause, view_any<literal>>;
    using factory_variable = container_factory<variable>;

private:
    const preprocessor& preprocessor_;
    factory_clause& clauses;
    factory_variable& variables;
    bool empty_clause;

public:
    formula(const preprocessor& preprocessor,
            factory_clause& clauses,
            factory_variable& variables) noexcept;

    std::size_t count_clause() const noexcept;
    std::size_t count_variable() const noexcept;

    variable& create_new_variable();

    void add_clause(view_any<literal> literals);
    void add_clause(auto... literals)
    {
        add_clause(view_any<literal>(std::array{literals...}));
    }

    bool for_each(std::function<bool(clause&)> f);
    bool for_each(std::function<bool(const clause&)> f) const;
    void for_each(std::function<void(variable&)> f);

    void write_DIMACS(std::ostream& output,
                      std::function<std::ostream&(std::ostream&, literal)>
                          transform = literal_printer) const;

    bool has_empty_clause();
};
}
