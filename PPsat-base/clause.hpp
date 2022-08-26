#pragma once
#include <PPsat-base/clause_category.hpp>
#include <PPsat-base/literal.hpp>
#include <PPsat-base/optional.hpp>

#include <functional>
#include <optional>

namespace PPsat_base
{
class clause
{
public:
    using category = clause_category;

    virtual void for_each(std::function<void(literal)> f) const = 0;

    virtual PPsat_base::optional<literal> is_unary_unit() const = 0;

    virtual std::pair<bool, std::optional<literal>> assign(
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

struct decisive_t
{};
constexpr inline decisive_t decisive;

bool operator==(clause::category category, decisive_t) noexcept;
}
