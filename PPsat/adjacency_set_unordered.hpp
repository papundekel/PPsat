#pragma once
#include <PPsat/adjacency.hpp>
#include <PPsat/variable.hpp>
#include <PPsat/variable_assignable.hpp>

#include <PPsat-base/virtual_base.hpp>

#include <functional>
#include <unordered_set>
#include <utility>

namespace PPsat
{
class adjacency_set_unordered : public virtual adjacency
{
    using element_t = std::pair<std::reference_wrapper<clause>, bool>;

    class hasher
    {
    public:
        std::size_t operator()(const element_t& e) const noexcept
        {
            return std::hash<decltype(&e.first.get())>()(&e.first.get());
        }
    };
    class comparer
    {
    public:
        bool operator()(const element_t& a, const element_t& b) const noexcept
        {
            return &a.first.get() == &b.first.get();
        }
    };

    std::unordered_set<element_t, hasher, comparer> adjacency;

public:
    void adjacent_for_each(std::function<bool(clause&, bool)> f) override final
    {
        for (auto i = adjacency.begin(); i != adjacency.end();)
        {
            const auto i_current = i++;
            auto& pair = *i_current;

            const auto remove = f(pair.first, pair.second);

            if (remove)
            {
                adjacency.erase(i_current);
            }
        }
    }

    void adjacent_add(clause& clause, bool positive) override final
    {
        adjacency.emplace(clause, positive);
    }

    void adjacent_remove(clause& clause, bool positive) override final
    {
        adjacency.erase(std::make_pair(std::ref(clause), positive));
    }

    std::size_t size() const override final
    {
        return adjacency.size();
    }
};
}
