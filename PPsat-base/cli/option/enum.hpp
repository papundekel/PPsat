#pragma once
#include <PPsat-base/cli/option.hpp>
#include <PPsat-base/view_any.hpp>

#include <algorithm>
#include <map>
#include <span>
#include <string_view>
#include <utility>

namespace PPsat_base::cli::option
{
class enum_ : public virtual option_
{
    std::map<std::string_view, int> mapping;

protected:
    int enumeration;

public:
    enum_(auto default_value, auto&& mapping)
        : mapping()
        , enumeration((int)default_value)
    {
        for (auto&& key_value : std::forward<decltype(mapping)>(mapping))
        {
            this->mapping.try_emplace(
                std::forward<decltype(key_value)>(key_value).first,
                (int)std::forward<decltype(key_value)>(key_value).second);
        }
    }

    std::size_t argument_count() const noexcept override final;

    bool parse(const logger& logger_outer,
               std::size_t i,
               std::string_view argument) noexcept override final;
};
}
