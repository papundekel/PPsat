#pragma once
#include <PPsat-base/clause_category.hpp>
#include <PPsat-base/variable_assignment.hpp>

#include <compare>
#include <iosfwd>
#include <optional>
#include <utility>

namespace PPsat_base
{
class variable;
class clause;

class literal
{
    variable* var;
    bool positive;

public:
    literal(const literal& literal, bool positive) noexcept;
    literal(PPsat_base::variable& variable, bool positive) noexcept;

    variable& get_variable() const noexcept;
    variable_assignment get_assignment() const noexcept;

    bool is_positive() const noexcept;

    literal operator!() const noexcept;
    std::strong_ordering operator<=>(const literal& other) const noexcept;
    bool operator==(const literal& other) const = default;

    void register_(clause& clause) const;
    void unregister(clause& clause) const;

    std::tuple<bool, std::optional<literal>, std::size_t> assign() const;
    void unassign() const;
};

std::ostream& operator<<(std::ostream& output, const literal& literal);

constexpr inline std::ostream& (&literal_printer)(std::ostream& output,
                                                  const literal& literal) =
operator<<;
}
