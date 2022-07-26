#pragma once
#include <PPsat/literal.hpp>

#include <iostream>
#include <vector>

namespace PPsat
{
template <typename ClauseInserter>
class builder_tseitin
{
    ClauseInserter clause_inserter;
    bool nnf;

public:
    builder_tseitin(ClauseInserter clause_inserter, bool nnf)
        : clause_inserter(std::move(clause_inserter))
        , nnf(nnf)
    {}

private:
    void push(const literal auto&... literals)
    {
        *clause_inserter++ = {std::forward<decltype(literals)>(literals)...};
    }

public:
    void push_conjunction(const literal auto& p,
                          const literal auto& q,
                          const literal auto& r)
    {
        if (!nnf)
        {
            push(p, !q, !r);
        }

        push(!p, q);
        push(!p, r);
    }

    void push_disjunction(const literal auto& p,
                          const literal auto& q,
                          const literal auto& r)
    {
        push(!p, q, r);

        if (!nnf)
        {
            push(p, !q);
            push(p, !r);
        }
    }

    void push_negation(const literal auto& p, const literal auto& q)
    {
        if (!nnf)
        {
            push(p, q);
        }

        push(!p, !q);
    }

    void push_literal(const literal auto& l)
    {
        push(l);
    }
};
}
