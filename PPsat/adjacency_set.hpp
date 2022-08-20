#pragma once
#include <PPsat/adjacency.hpp>

#include <functional>
#include <set>
#include <utility>

namespace PPsat
{
class adjacency_set : public virtual adjacency
{
    using element_t =
        std::pair<std::reference_wrapper<PPsat_base::clause>, bool>;

    class comparer
    {
    public:
        bool operator()(const element_t&, const element_t&) const noexcept;
    };

    std::set<element_t, comparer> adjacency;

    void adjacent_for_each(
        std::function<void(PPsat_base::clause&, bool)> f) const override final;

    void adjacent_add(PPsat_base::clause& clause, bool positive) override final;
    void adjacent_remove(PPsat_base::clause& clause,
                         bool positive) override final;
};
}
