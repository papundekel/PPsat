#pragma once

#include <PPsat-base/variable.hpp>

#include <antlr4-runtime.h>

#include <string_view>

namespace PPsat
{
class renaming
{
public:
    virtual PPsat_base::variable* contains(
        std::string_view name_input) const = 0;

    virtual void emplace(std::string_view name_input,
                         PPsat_base::variable& variable) = 0;

    virtual ~renaming() = default;
};
}
