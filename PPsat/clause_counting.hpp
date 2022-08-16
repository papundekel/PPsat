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
class clause_counting final : public clause_no_add
{
    std::vector<PPsat_base::literal> literals;
    std::size_t unassigned;
    std::size_t assigned_false;
    std::size_t assigned_true;
    PPsat_base::literal literal_unit;

public:
    clause_counting(PPsat_base::view_any<const PPsat_base::literal> literals);

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
