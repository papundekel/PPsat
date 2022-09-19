#pragma once
#include <PPsat/renaming.hpp>
#include <PPsat/variable.hpp>

#include <antlr4-runtime.h>

#include <string_view>

namespace PPsat
{
class renaming_int : public renaming
{
public:
    static std::optional<std::size_t> parse_number(std::string_view name_input);

    variable* contains(std::string_view name_input) const override final;
    virtual variable* contains(std::size_t name_input) const = 0;

    void emplace(std::string_view name_input,
                 variable& variable) override final;
    virtual void emplace(std::size_t name_input, variable& variable) = 0;
};
}
