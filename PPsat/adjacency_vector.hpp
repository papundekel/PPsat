#pragma once
#include <PPsat/adjacency.hpp>

#include <PPsat-base/variable.hpp>

#include <functional>
#include <utility>
#include <vector>

namespace PPsat
{
class adjacency_vector : public virtual adjacency
{
    std::vector<std::pair<std::reference_wrapper<PPsat_base::clause>, bool>>
        adjacency;

    void adjacent_for_each(
        std::function<void(PPsat_base::clause&, bool)> f) const override final;

    void adjacent_add(PPsat_base::clause& clause, bool positive) override final;
    void adjacent_remove(PPsat_base::clause& clause,
                         bool positive) override final;
};
}
