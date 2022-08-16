#pragma once
#include <PPsat/clause_no_add.hpp>

#include <PPsat-base/formula.hpp>
#include <PPsat-base/literal.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

namespace PPsat
{
class clause_sets final : public clause_no_add
{
    using set = std::set<PPsat_base::literal>;

    set unassigned;
    set assigned_false;
    set assigned_true;

public:
    clause_sets(PPsat_base::view_any<const PPsat_base::literal> literals);

    void for_each(
        std::function<void(PPsat_base::literal)> f) const override final;
    std::size_t length() const noexcept override final;

    bool is_sat() const noexcept override final;

    std::pair<category, PPsat_base::literal>
    get_category_and_first_literal_impl() const noexcept override final;

    void assign(PPsat_base::literal literal_assigned,
                bool positive_in_clause) override final;

    void unassign(PPsat_base::literal literal_unassigned,
                  bool positive_in_clause) override final;
};
}
