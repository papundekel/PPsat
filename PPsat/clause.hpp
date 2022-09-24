#pragma once
#include <PPsat/clause_category.hpp>
#include <PPsat/literal.hpp>

#include <PPsat-base/optional.hpp>

#include <compare>
#include <functional>
#include <optional>

namespace PPsat
{
class clause
{
public:
    enum class type
    {
        basic,
        counting,
        watched_literals,
    };

    using category = clause_category;

    virtual void reset() = 0;

    virtual void for_each(std::function<void(literal)> f) const = 0;

    virtual PPsat_base::optional<literal> is_unary_unit() const = 0;

    virtual std::tuple<bool, bool, std::optional<literal>> assign(
        literal literal_assigned,
        bool positive_in_clause) = 0;
    virtual void unassign(literal literal_unassigned,
                          bool positive_in_clause) = 0;

    virtual bool is_relevant(literal literal) const = 0;
    virtual bool antecedent_to_some() const = 0;
    virtual void unregister() = 0;

    virtual std::size_t length() const = 0;

    virtual ~clause() = default;
};

constexpr std::strong_ordering operator<=>(const clause::type& a,
                                           const clause::type& b)
{
    return (int)a <=> (int)b;
}

struct decisive_t
{};
constexpr inline decisive_t decisive;

bool operator==(clause::category category, decisive_t) noexcept;
}
