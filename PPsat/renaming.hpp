#pragma once
#include <PPsat/variable.hpp>

#include <antlr4-runtime.h>

#include <string_view>

namespace PPsat
{
class renaming
{
public:
    virtual variable* contains(std::string_view name_input) const = 0;

    virtual void emplace(std::string_view name_input, variable& variable) = 0;

    virtual ~renaming() = default;
};
}
