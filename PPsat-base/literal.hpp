#pragma once
#include <compare>
#include <iosfwd>

namespace PPsat_base
{
class variable;
class clause;

class literal
{
    variable* var;
    bool positive;

public:
    literal() = default;
    literal(const literal& literal, bool positive) noexcept;
    literal(PPsat_base::variable& variable, bool positive) noexcept;

    variable& get_variable() const noexcept;

    bool is_positive() const noexcept;

    literal operator!() const noexcept;
    std::strong_ordering operator<=>(const literal& other) const noexcept;

    void register_containing_clause(clause& clause) const;
};

std::ostream& operator<<(std::ostream& output, const literal& literal);

constexpr inline std::ostream& (&literal_printer)(std::ostream& output,
                                                  const literal& literal) =
operator<<;
}
