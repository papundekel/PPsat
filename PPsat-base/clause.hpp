#pragma once
#include <PPsat-base/literal.hpp>

#include <functional>

namespace PPsat_base
{
class clause
{
public:
    enum class category
    {
        other,
        unit,
        sat,
        unsat
    };

private:
    virtual std::pair<category, literal> get_category_and_first_literal_impl()
        const noexcept = 0;

public:
    virtual void for_each(std::function<void(literal)> f) const = 0;
    virtual std::size_t length() const noexcept = 0;

    virtual void add_literal(literal literal) = 0;

    virtual bool is_sat() const noexcept = 0;

    std::pair<category, literal> get_category_and_first_literal()
        const noexcept;

    virtual void assign(literal literal_assigned, bool positive_in_clause) = 0;
    virtual void unassign(literal literal_unassigned,
                          bool positive_in_clause) = 0;
};

struct decisive_t
{};
constexpr inline decisive_t decisive;

bool operator==(clause::category category, decisive_t) noexcept;
}
