#pragma once
#include <PPsat-base/clause.hpp>

#include <PPsat-base/formula.hpp>
#include <PPsat-base/literal.hpp>

namespace PPsat
{
class clause_watched_literals final : public PPsat_base::clause
{
    using literals_t = std::vector<PPsat_base::literal>;

    literals_t literals;
    PPsat_base::literal watched1;
    PPsat_base::literal watched2;

public:
    clause_watched_literals(PPsat_base::view_any<PPsat_base::literal> literals);

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
