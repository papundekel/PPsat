#pragma once
#include <PPsat/adjacency.hpp>
#include <PPsat/variable_adjacency.hpp>

#include <PPsat-base/clause.hpp>

#include <functional>

namespace PPsat
{
template <bool yes>
class variable_unassigning
    : public virtual PPsat_base::variable
    , public virtual adjacency
{
private:
    void for_each_clause_relevant_unassign(
        std::function<void(PPsat_base::clause&, bool)> f) const override final
    {
        if constexpr (yes)
        {
            adjacent_for_each(std::move(f));
        }
    }
};
}
