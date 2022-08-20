#pragma once
#include <PPsat/adjacency.hpp>

#include <PPsat-base/clause.hpp>
#include <PPsat-base/variable.hpp>

#include <functional>

namespace PPsat
{
class variable_adjacency
    : public virtual PPsat_base::variable
    , public virtual adjacency
{
private:
    void for_each_clause_relevant_assign(
        std::function<void(PPsat_base::clause&, bool)> f) const override final;
    void register_(PPsat_base::clause& clause, bool positive) override final;
    void unregister(PPsat_base::clause& clause, bool positive) override final;
};
}
