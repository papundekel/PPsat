#pragma once
#include "PPsat-base/literal.hpp"
#include <PPsat-base/clause.hpp>
#include <PPsat-base/view_any.hpp>

namespace PPsat_crypt
{
class clause : public PPsat_base::clause
{
public:
    clause(PPsat_base::view_any<const PPsat_base::literal> literals);

private:
    void for_each(
        std::function<void(PPsat_base::literal)> f) const override final;

    std::size_t length() const noexcept override final;

    void add_literal(PPsat_base::literal literal) override final;

    bool is_sat() const noexcept override final;

    std::pair<category, PPsat_base::literal>
    get_category_and_first_literal_impl() const noexcept override final;

    void assign(PPsat_base::literal literal_assigned,
                bool positive_in_clause) override final;

    void unassign(PPsat_base::literal literal_unassigned,
                  bool positive_in_clause) override final;
};
}
