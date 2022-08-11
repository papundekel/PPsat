#pragma once
#include <PPsat/formula.hpp>
#include <PPsat/literal_pair.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

namespace PPsat
{
class clause_simple final : public clause
{
    std::vector<literal_pair> literals;

public:
    clause_simple(view_any<const literal> literals);

    void for_each(std::function<void(const literal&)> f) const override final;
    std::size_t length() const noexcept override final;
};

class formula_simple : public formula
{
public:
    std::vector<clause_simple> clauses;

    void add_clause(view_any<const literal> literals) override final;

    void for_each(std::function<void(const clause&)> f) const override final;

    std::size_t clause_count() const noexcept override final;
};
}
