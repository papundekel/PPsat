#pragma once
#include <PPsat-base/clause.hpp>
#include <PPsat-base/formula.hpp>
#include <PPsat-base/literal.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

namespace PPsat
{
class clause_sets final : public PPsat_base::clause
{
    using set = std::set<PPsat_base::literal>;

    set unassigned;
    set assigned_false;
    set assigned_true;

public:
    clause_sets(PPsat_base::view_any<const PPsat_base::literal> literals);

    void for_each(
        std::function<void(PPsat_base::literal)> f) const override final;

    std::optional<PPsat_base::literal> is_unit() const override final;

    std::pair<bool, std::optional<PPsat_base::literal>> assign(
        PPsat_base::literal literal_assigned,
        bool positive_in_clause) override final;

    void unassign(PPsat_base::literal literal_unassigned,
                  bool positive_in_clause) override final;

    bool is_relevant(PPsat_base::literal literal) const override final;
};
}
