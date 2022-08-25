#pragma once
#include "PPsat/adjacency.hpp"
#include <PPsat/variable_assignable.hpp>

#include <PPsat-base/variable.hpp>

#include <functional>
#include <unordered_set>
#include <utility>

namespace PPsat
{
class adjacency_set_unordered : public virtual adjacency
{
    using element_t =
        std::pair<std::reference_wrapper<PPsat_base::clause>, bool>;

    class hasher
    {
    public:
        std::size_t operator()(const element_t&) const noexcept;
    };
    class comparer
    {
    public:
        bool operator()(const element_t&, const element_t&) const noexcept;
    };

    std::unordered_set<element_t, hasher, comparer> adjacency;

    void adjacent_for_each(
        std::function<void(PPsat_base::clause&, bool)> f) const override final;

    void adjacent_add(PPsat_base::clause& clause, bool positive) override final;
    void adjacent_remove(PPsat_base::clause& clause,
                         bool positive) override final;
    std::size_t size() const override final;
};
}
