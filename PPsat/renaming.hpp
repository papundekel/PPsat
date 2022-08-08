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
    virtual std::string_view rename(std::size_t name_native,
                                    std::string&& name_input) = 0;
    virtual void rename(std::size_t name_native, std::size_t name_input) = 0;

    virtual std::optional<name> get(std::size_t name_internal) const = 0;

    virtual std::size_t count() const = 0;

    virtual ~renaming() = default;
};
}
