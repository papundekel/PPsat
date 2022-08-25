#pragma once

#include "PPsat/renaming.hpp"
#include <PPsat-base/variable.hpp>

#include <antlr4-runtime.h>

#include <string_view>

namespace PPsat
{
class renaming_int : public renaming
{
public:
    static std::optional<std::size_t> parse_number(std::string_view name_input);

    PPsat_base::variable* contains(
        std::string_view name_input) const override final;
    virtual PPsat_base::variable* contains(std::size_t name_input) const = 0;

    void emplace(std::string_view name_input,
                 PPsat_base::variable& variable) override final;
    virtual void emplace(std::size_t name_input,
                         PPsat_base::variable& variable) = 0;
};
}
