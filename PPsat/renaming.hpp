#pragma once
#include <PPsat/literal.hpp>

#include <compare>
#include <iosfwd>
#include <optional>
#include <string_view>

namespace PPsat
{
std::ostream& operator<<(std::ostream& output, class renamed_variable variable);
std::ostream& operator<<(std::ostream& output, class renamed_literal literal);

class renamed_variable
{
    std::string_view string;
    std::size_t number;
    bool has_string;

public:
    renamed_variable(std::string_view string) noexcept;
    renamed_variable(std::size_t number) noexcept;

    bool operator<(const renamed_variable& other) const noexcept;

    friend std::ostream& operator<<(std::ostream& output,
                                    renamed_variable name);
};

class renamed_literal
{
public:
    renamed_variable variable;
    bool is_positive;

    friend std::ostream& operator<<(std::ostream& output, renamed_literal name);

    bool operator<(const renamed_literal& other) const noexcept;
};

class renaming
{
public:
    virtual std::string_view rename(std::size_t name_native,
                                    std::string&& name_input) = 0;
    virtual void rename(std::size_t name_native, std::size_t name_input) = 0;

    virtual std::optional<renamed_variable> get_variable(
        std::size_t name_internal) const = 0;

    std::optional<renamed_literal> get_literal(const literal& literal) const;

    virtual std::size_t count() const = 0;

    virtual ~renaming() = default;
};
}
