#pragma once
#include <PPsat/clause.hpp>
#include <PPsat/cli/parameters.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/preprocessor.hpp>
#include <PPsat/unit.hpp>
#include <PPsat/variable.hpp>

#include <PPsat-base/factory_container.hpp>
#include <PPsat-base/optional.hpp>
#include <PPsat-base/view_any.hpp>

#include <functional>
#include <ranges>

namespace PPsat
{
class formula
{
public:
    using factory_clause =
        PPsat_base::factory_container<clause, PPsat_base::view_any<literal>>;
    using factory_variable = PPsat_base::factory_container<variable>;

    virtual std::size_t count_clause() const noexcept = 0;
    virtual std::size_t count_variable() const noexcept = 0;

    virtual variable& create_new_variable() = 0;

    virtual void add_clause(PPsat_base::view_any<literal> literals) = 0;
    void add_clause(auto... literals)
    {
        add_clause(PPsat_base::view_any<literal>(std::array{literals...}));
    }

    virtual bool for_each_clause(std::function<bool(clause&)> f) = 0;
    virtual bool for_each_clause(
        std::function<bool(const clause&)> f) const = 0;

    virtual void for_each_variable(std::function<void(variable&)> f) = 0;

    virtual void write_DIMACS(
        std::ostream& output,
        std::function<std::ostream&(std::ostream&, literal)> transform =
            literal_printer) const = 0;

    virtual std::list<unit> find_unary_unit() const = 0;

    virtual bool has_empty_clause() = 0;

    virtual ~formula() = default;
};
}
