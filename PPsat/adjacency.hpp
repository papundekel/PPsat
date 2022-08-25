#pragma once
#include <functional>

namespace PPsat_base
{
class clause;
}

namespace PPsat
{
class adjacency
{
protected:
    virtual void adjacent_for_each(
        std::function<void(PPsat_base::clause&, bool)> f) const = 0;
    virtual void adjacent_add(PPsat_base::clause& clause, bool positive) = 0;
    virtual void adjacent_remove(PPsat_base::clause& clause, bool positive) = 0;
    virtual std::size_t size() const = 0;

    virtual ~adjacency() = default;
};
}
