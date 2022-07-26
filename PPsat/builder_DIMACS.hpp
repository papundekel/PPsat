#pragma once
#include <PPsat/literal.hpp>

#include <iostream>
#include <vector>
#include <map>

namespace PPsat
{
template <typename ClauseInserter>
class builder_DIMACS
{
    ClauseInserter clause_inserter;
    std::map<std::size_t, std::size_t> renaming;
    std::size_t name_next;

public:
    builder_DIMACS(ClauseInserter clause_inserter)
        : clause_inserter(std::move(clause_inserter))
        , renaming()
        , name_next(0)
    {}

    void push_clause(auto clause)
    {
        for (auto& literal : clause)
        {
            if (const auto i = renaming.find(literal.variable); i != renaming.end())
            {
                literal.variable = i->second;
            }
            else
            {
                const auto rename = name_next++;

                renaming.try_emplace(literal.variable, rename);
                literal.variable = rename;
            }
        }

        *clause_inserter++ = std::move(clause);
    }

    auto get_renaming() &&
    {
        return std::move(renaming);
    }
};
}
