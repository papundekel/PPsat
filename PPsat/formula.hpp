#pragma once
#include <PPsat/iterator_any.hpp>
#include <PPsat/literal.hpp>

#include <functional>
#include <ranges>

namespace PPsat
{
class clause
{
public:
    virtual void for_each(std::function<void(const literal&)> f) const = 0;
};

using view_literal = view_any<const literal>;

class formula
{
public:
    virtual void add_clause(view_literal literals) = 0;
    virtual void for_each(std::function<void(const clause&)> f) const = 0;
    virtual std::size_t clause_count() const = 0;

    void add_clause(const auto&... literals)
    {
        add_clause(view_literal(
            std::array{std::reference_wrapper<const literal>(literals)...} |
            std::views::transform([](auto& x) -> auto& { return x.get(); })));
    }

    void write_DIMACS(
        std::ostream& output,
        std::function<std::ostream&(std::ostream&, const literal&)> transform)
        const
    {
        for_each(
            [&output, &transform](const clause& c)
            {
                c.for_each(
                    [&output, &transform](const literal& l)
                    {
                        transform(output, l) << " ";
                    });
                output << "0\n";
            });
    }
};
}
