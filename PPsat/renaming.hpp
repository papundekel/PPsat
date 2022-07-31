#pragma once
#include <map>
#include <optional>
#include <ranges>
#include <string_view>

namespace PPsat
{
class renaming
{
public:
    virtual void rename(std::string_view input_name,
                        std::size_t internal_name) = 0;

    virtual std::optional<std::string_view> get(std::size_t internal_name) const = 0;

    virtual void for_each(
        std::function<void(std::size_t, std::string_view)> f) const = 0;

    virtual std::size_t count() const = 0;
};
}
