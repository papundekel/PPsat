#pragma once
#include <iterator>
#include <iostream>

namespace PPsat
{
template <template <typename> typename Formula,
          template <typename>
          typename Clause,
          typename Literal>
class CNF
{
public:
    Formula<Clause<Literal>> formula;

    auto clause_inserter()
    {
        return std::back_inserter(formula);
    }

    void write_DIMACS(std::ostream& output, auto&& transform) const
    {
        for (const auto& clause : formula)
        {
            for (const auto& literal : clause)
            {
                output << std::forward<decltype(transform)>(transform)(literal) << " ";
            }
            output << "0\n";
        }
    }

    auto clauses_count() const noexcept
    {
        return formula.size();
    }
};
}
