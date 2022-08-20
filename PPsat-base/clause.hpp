#pragma once
#include <PPsat-base/clause_category.hpp>
#include <PPsat-base/literal.hpp>

#include <functional>
#include <optional>

namespace PPsat_base
{
class clause
{
public:
    using category = clause_category;

    virtual void for_each(std::function<void(literal)> f) const = 0;

    virtual std::optional<literal> is_unit() const = 0;

    virtual std::pair<bool, std::optional<literal>> assign(
        literal literal_assigned,
        bool positive_in_clause) = 0;
    virtual void unassign(literal literal_unassigned,
                          bool positive_in_clause) = 0;

    virtual bool is_relevant(literal literal) const = 0;
};

struct decisive_t
{};
constexpr inline decisive_t decisive;

bool operator==(clause::category category, decisive_t) noexcept;
}
