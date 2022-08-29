#pragma once
#include <PPsat-base/clause.hpp>

#include <PPsat-base/formula.hpp>
#include <PPsat-base/literal.hpp>

namespace PPsat
{
class clause_unary final : public PPsat_base::clause
{
    PPsat_base::literal literal;

public:
    clause_unary(PPsat_base::literal literal);

    void for_each(
        std::function<void(PPsat_base::literal)> f) const override final;

    PPsat_base::optional<PPsat_base::literal> is_unary_unit()
        const override final;

    std::pair<bool, std::optional<PPsat_base::literal>> assign(
        PPsat_base::literal literal_assigned,
        bool positive_in_clause) override final;

    void unassign(PPsat_base::literal literal_unassigned,
                  bool positive_in_clause) override final;

    bool antecedent_to_some() const override final;
    void unregister() override final;
    std::size_t length() const override final;

    bool is_relevant(PPsat_base::literal literal) const override final;
};
}
