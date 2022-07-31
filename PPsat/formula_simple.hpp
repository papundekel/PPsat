#pragma once
#include <PPsat/formula.hpp>
#include <PPsat/literal_pair.hpp>

#include <iostream>
#include <iterator>

namespace PPsat
{
class clause_simple : public clause
{
    const std::vector<literal_pair>& literals;

public:
    clause_simple(const std::vector<literal_pair>& literals)
        : literals(literals)
    {}

    void for_each(std::function<void(const literal&)> f) const override final
    {
        std::ranges::for_each(literals, f);
    }
};

class formula_simple : public formula
{
public:
    std::vector<std::vector<literal_pair>> clauses;

    void add_clause(any_view<const literal> literals) override final
    {
        clauses.push_back({literals.begin(), literals.end()});
    }

    void for_each(std::function<void(const clause&)> f) const override final
    {
        std::ranges::for_each(clauses,
                              [&f](auto& c)
                              {
                                  f(clause_simple(c));
                              });
    }

    std::size_t clause_count() const noexcept override final
    {
        return clauses.size();
    }
};
}
