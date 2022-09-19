#pragma once
#include <PPsat/clause_category.hpp>
#include <PPsat/variable_assignment.hpp>

#include <PPsat-base/optional.hpp>

#include <compare>
#include <iosfwd>
#include <list>
#include <optional>
#include <utility>

namespace PPsat
{
class variable;
class clause;
class unit;

class literal
{
    variable* var;
    bool positive;

public:
    literal(const literal& literal, bool positive) noexcept;
    literal(variable& variable, bool positive) noexcept;

    variable& get_variable() const noexcept;
    variable_assignment assignment_get() const noexcept;

    bool is_positive() const noexcept;

    literal operator!() const noexcept;
    std::strong_ordering operator<=>(const literal& other) const noexcept;
    bool operator==(const literal& other) const = default;

    void register_(clause& clause) const;
    void unregister(clause& clause) const;

    std::
        tuple<PPsat_base::optional<const clause&>, std::list<unit>, std::size_t>
        assign(std::size_t level, std::size_t recency) const;
    void unassign() const;

    std::size_t level_get() const;
    std::size_t recency_get() const;
    PPsat_base::optional<const clause&> antecedent_get() const;
};

std::ostream& operator<<(std::ostream& output, const literal& literal);

constexpr inline std::ostream& (&literal_printer)(std::ostream& output,
                                                  const literal& literal) =
operator<<;
}
