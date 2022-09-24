#pragma once
#include <PPsat/adjacency.hpp>
#include <PPsat/variable.hpp>
#include <PPsat/variable_assignable.hpp>

#include <PPsat-base/virtual_base.hpp>

#include <functional>
#include <list>
#include <utility>

namespace PPsat
{
class adjacency_list : public virtual adjacency
{
    std::list<std::pair<std::reference_wrapper<clause>, bool>> adjacency;

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
        adjacency.emplace_back(clause, positive);
    }

    void adjacent_remove(clause& clause, bool positive) override final
    {
        const auto i = std::ranges::find(adjacency,
                                         &clause,
                                         [](const auto& pair)
                                         {
                                             return &pair.first.get();
                                         });

        adjacency.erase(i);
    }

    void adjacent_reset() override final
    {
        adjacency.clear();
    }

    std::size_t size() const override final
    {
        return adjacency.size();
    }
};
}
