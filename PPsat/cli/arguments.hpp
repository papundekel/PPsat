#pragma once
#include "PPsat/logger_subroutine.hpp"
#include <PPsat/cli/argument.hpp>

#include <string_view>

namespace PPsat::cli
{
class arguments
{
    virtual const std::string_view* begin() const noexcept = 0;
    virtual const std::string_view* end() const noexcept = 0;

public:
    virtual void add(std::string_view argument) noexcept = 0;

    bool parse(const logger& logger_outer, auto&& arguments) const noexcept
    {
        const auto& logger_inner =
            logger_subroutine(logger_outer, "cli_argument");

        bool success = true;

        auto arguments_i =
            std::begin(std::forward<decltype(arguments)>(arguments));
        const auto arguments_end =
            std::end(std::forward<decltype(arguments)>(arguments));

        auto argument_strings_i = begin();
        const auto argument_strings_end = end();

        for (; arguments_i != arguments_end &&
               argument_strings_i != argument_strings_end;
             ++arguments_i, ++argument_strings_i)
        {
            argument_& argument = *arguments_i;

            const auto success_local =
                argument.parse(logger_inner, *argument_strings_i);

            if (success_local)
            {
                argument.set_presence();
            }
            else
            {
                logger_inner << "Parsing an argument failed.\n";
                return false;
            }
        }

        if (arguments_i == arguments_end &&
            argument_strings_i != argument_strings_end)
        {
            logger_inner << "Too many arguments provided.\n";
            return false;
        }

        return true;
    }
};
}
