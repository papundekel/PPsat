#pragma once
#include <PPsat/clause.hpp>
#include <PPsat/container_factory.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/variable.hpp>
#include <PPsat/view_any.hpp>

#include <functional>
#include <ranges>

namespace PPsat
{
using view_literal = view_any<const literal>;

class formula
{
public:
    using factory_clause = container_factory<clause, view_any<const literal>>;
    using factory_variable = container_factory<variable>;

private:
    factory_clause& clauses;
    factory_variable& variables;

public:
    formula(factory_clause& clauses, factory_variable& variables) noexcept;

    std::size_t clause_count() const noexcept;

    variable& create_new_variable();

    void add_clause(view_literal literals);
    void add_clause(const auto&... literals)
    {
        add_clause(view_literal(
            std::array{std::reference_wrapper<const literal>(literals)...} |
            std::views::transform([](auto& x) -> auto& { return x.get(); })));
    }

    bool for_each(std::function<bool(clause&)> f);
    bool for_each(std::function<bool(const clause&)> f) const;
    void for_each(std::function<void(variable&)> f);

    void write_DIMACS(std::ostream& output,
                      std::function<std::ostream&(std::ostream&, literal)>
                          transform = literal_printer) const;

    bool contains_unsat_clause() const noexcept;
    std::optional<literal> find_unit() const noexcept;
};
}
