#pragma once
#include "PPsat/adjacency.hpp"
#include <PPsat/variable_assignable.hpp>

#include <PPsat-base/variable.hpp>

#include <functional>
#include <list>
#include <utility>

namespace PPsat
{
class adjacency_list : public virtual adjacency
{
    std::list<std::pair<std::reference_wrapper<PPsat_base::clause>, bool>>
        adjacency;

    void adjacent_for_each(
        std::function<void(PPsat_base::clause&, bool)> f) const override final;

    void adjacent_add(PPsat_base::clause& clause, bool positive) override final;
    void adjacent_remove(PPsat_base::clause& clause,
                         bool positive) override final;
    std::size_t size() const override final;
};
}
