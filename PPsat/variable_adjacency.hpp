#pragma once
#include "PPsat-base/template_t.hpp"
#include <PPsat/adjacency.hpp>
#include <PPsat/clause.hpp>
#include <PPsat/variable.hpp>

#include <PPsat-base/virtual_base.hpp>

#include <functional>

namespace PPsat
{
template <bool virtual_, auto... Bases>
class variable_adjacency : public PPsat_base::virtual_base<virtual_, Bases...>
{
public:
    void for_each_clause_relevant_assign(std::function<bool(clause&, bool)> f)
    {
        this->adjacent_for_each(std::move(f));
    }

    void register_(clause& clause, bool positive)
    {
        this->adjacent_add(clause, positive);
    }

    void unregister(clause& clause, bool positive)
    {
        this->adjacent_remove(clause, positive);
    }

    std::size_t adjacency_size() const
    {
        return this->size();
    }
};
}
