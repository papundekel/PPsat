#pragma once
#include "PPsat-base/virtual_base.hpp"
#include <PPsat/adjacency.hpp>
#include <PPsat/clause.hpp>
#include <PPsat/variable_adjacency.hpp>

#include <functional>

namespace PPsat
{
template <bool yes, bool virtual_, auto... Bases>
class variable_unassigning : public PPsat_base::virtual_base<virtual_, Bases...>
{
public:
    void for_each_clause_relevant_unassign(
        std::function<void(clause&, bool)> f) override final
    {
        if constexpr (yes)
        {
            this->adjacent_for_each(
                [&f](clause& clause, bool positive)
                {
                    f(clause, positive);
                    return false;
                });
        }
    }
};
}
