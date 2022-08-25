#pragma once
#include <PPsat-base/clause.hpp>
#include <PPsat-base/formula.hpp>
#include <PPsat-base/literal.hpp>
#include <PPsat-base/optional.hpp>

namespace PPsat
{
class clause_counting final : public PPsat_base::clause
{
    std::vector<PPsat_base::literal> literals;
    std::size_t unassigned;
    std::size_t assigned_false;
    std::size_t assigned_true;

public:
    clause_counting(PPsat_base::view_any<PPsat_base::literal> literals);

    void for_each(
        std::function<void(PPsat_base::literal)> f) const override final;

    PPsat_base::optional<PPsat_base::literal> is_unary_unit()
        const override final;

    std::pair<bool, std::optional<PPsat_base::literal>> assign(
        PPsat_base::literal literal_assigned,
        bool positive_in_clause) override final;

    void unassign(PPsat_base::literal literal_unassigned,
                  bool positive_in_clause) override final;

    bool is_relevant(PPsat_base::literal literal) const override final;
};
}
