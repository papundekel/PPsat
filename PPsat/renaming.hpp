#pragma once
#include <iosfwd>
#include <optional>
#include <string_view>

namespace PPsat
{
class name;
std::ostream& operator<<(std::ostream& output, name name);

class name
{
    std::string_view string;
    std::size_t number;
    bool has_string;

public:
    name(std::string_view string) noexcept;
    name(std::size_t number) noexcept;

    friend std::ostream& operator<<(std::ostream& output, name name);
};

class renaming
{
public:
    virtual void rename(std::string_view input_name,
                        std::size_t internal_name) = 0;
    virtual void rename(std::size_t input_name, std::size_t internal_name) = 0;

    virtual std::optional<name> get(std::size_t internal_name) const = 0;

    virtual std::size_t count() const = 0;

    virtual ~renaming() = default;
};
}
