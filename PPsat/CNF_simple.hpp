#pragma once
#include <iostream>
#include <vector>

namespace PPsat
{
template <template <typename> typename S, typename L>
class CNF_simple
{
    S<S<L>> clauses;

public:
    void push_conjunction(L p, L q, L r)
    {
        clauses.push_back({p, -q, -r});
        clauses.push_back({-p, q});
        clauses.push_back({-p, r});
    }
    void push_disjunction(L p, L q, L r)
    {
        clauses.push_back({-p, q, r});
        clauses.push_back({p, -q});
        clauses.push_back({p, -r});
    }
    void push_literal(L l)
    {
        clauses.push_back({l});
    }

    void write_DIMACS(std::ostream& output) const
    {
        for (const auto& clause : clauses)
        {
            for (const auto& literal : clause)
            {
                output << literal << " ";
            }
            output << "0\n";
        }
    }

    std::size_t clauses_count() const noexcept
    {
        return clauses.size();
    }
};
}
