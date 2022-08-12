#pragma once
#include <PPsat/formula.hpp>
#include <PPsat/literal.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>

namespace PPsat
{
class clause_simple final : public clause
{
    std::list<literal> literals;

public:
    clause_simple(view_any<const literal> literals);

private:
    void for_each(std::function<void(literal)> f) const override final;
    std::size_t length() const noexcept override final;

    bool is_sat() const noexcept override final;

    std::pair<clause::category, literal> get_category_and_first_literal_impl()
        const noexcept override final;

    void assign(literal literal_assigned,
                bool positive_in_clause) override final;

    void unassign(literal literal_unassigned,
                  bool positive_in_clause) override final;
};
}
