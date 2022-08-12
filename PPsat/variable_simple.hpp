#pragma once
#include <PPsat/variable_name_variant.hpp>

namespace PPsat
{
class variable_simple final : public variable_name_variant
{
public:
    void for_each_clause_containing(
        std::function<void(clause&, bool)> f) const override final;

    void register_containing_clause(clause& clause,
                                    bool positive) override final;
};
}
