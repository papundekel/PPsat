#pragma once
#include <PPsat/formula.hpp>
#include <PPsat/literal.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

namespace PPsat
{
class clause_sets final : public clause
{
    using set = std::set<literal>;

    set unassigned;
    set assigned_false;
    set assigned_true;

public:
    clause_sets(view_any<const literal> literals);

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
