#pragma once
#include <PPsat/literal.hpp>

#include <iostream>
#include <vector>

namespace PPsat
{
template <template <typename> typename Formula, template <typename> typename Clause, literal Literal>
class CNF_simple
{
    Formula<Clause<Literal>> clauses;

public:
    void push_conjunction(const literal auto& p, const literal auto& q, const literal auto& r, bool nnf)
    {
        if (!nnf)
        {
            clauses.push_back({p, !q, !r});
        }
        
        clauses.push_back({!p, q});
        clauses.push_back({!p, r});
    }

    void push_disjunction(const literal auto& p, const literal auto& q, const literal auto& r, bool nnf)
    {
        clauses.push_back({!p, q, r});
        
        if (!nnf)
        {
            clauses.push_back({p, !q});
            clauses.push_back({p, !r});
        }
    }

    void push_negation(const literal auto& p, const literal auto& q, bool nnf)
    {
        if (!nnf)
        {
            clauses.push_back({p, q});
        }

        clauses.push_back({!p, !q});
    }

    void push_literal(const literal auto& l)
    {
        clauses.push_back({l});
    }

    void write_DIMACS(std::ostream& output, auto&& transform) const
    {
        for (const auto& clause : clauses)
        {
            for (const auto& literal : clause)
            {
                output << std::forward<decltype(transform)>(transform)(literal) << " ";
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
