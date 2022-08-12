#pragma once
#include "PPsat/variable_name_variant.hpp"
#include <PPsat/variable.hpp>

#include <functional>
#include <utility>
#include <vector>

namespace PPsat
{
class variable_adjacency final : public variable_name_variant
{
    std::vector<std::pair<std::reference_wrapper<clause>, bool>> adjacency;

public:
    void for_each_clause_containing(
        std::function<void(clause&, bool)> f) const override final;

    void register_containing_clause(clause& clause,
                                    bool positive) override final;
};
}
