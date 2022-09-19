#pragma once
#include <PPsat/clause.hpp>
#include <PPsat/literal.hpp>

#include <PPsat-base/view_any.hpp>
#include <PPsat-base/virtual_base.hpp>

#include <algorithm>

namespace PPsat
{
class clause_simple final : public clause
{
    std::vector<literal> literals;

public:
    clause_simple(PPsat_base::view_any<literal> literals)
        : literals(literals.begin(), literals.end())
    {}

    void for_each(std::function<void(literal)> f) const override final
    {
        std::ranges::for_each(literals, f);
    }

    PPsat_base::optional<PPsat::literal> is_unary_unit() const override final
    {
        return {};
    }

    std::tuple<bool, bool, std::optional<PPsat::literal>> assign(literal, bool)
        override final
    {
        return {false, false, {}};
    }

    void unassign(literal, bool) override final
    {}

    bool is_relevant(literal literal) const override final
    {
        return false;
    }

    bool antecedent_to_some() const override final
    {
        return false;
    }

    void unregister() override final
    {}

    std::size_t length() const override final
    {
        return 0;
    }
};
}
