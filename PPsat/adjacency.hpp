#pragma once
#include <compare>
#include <functional>

namespace PPsat
{
class clause;

class adjacency
{
public:
    enum class type
    {
        vector,
        list,
        set,
        set_unordered,
    };

protected:
    virtual void adjacent_for_each(std::function<bool(clause&, bool)> f) = 0;
    virtual void adjacent_add(clause& clause, bool positive) = 0;
    virtual void adjacent_remove(clause& clause, bool positive) = 0;
    virtual void adjacent_reset() = 0;
    virtual std::size_t size() const = 0;

    virtual ~adjacency() = default;
};

constexpr std::strong_ordering operator<=>(const adjacency::type& a,
                                           const adjacency::type& b)
{
    return (int)a <=> (int)b;
}
}
