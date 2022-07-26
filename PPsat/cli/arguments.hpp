#pragma once
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
    bool parse(auto&& arguments) const noexcept
    {
        bool success = true;

        auto arguments_i = std::begin(std::forward<decltype(arguments)>(arguments));
        const auto arguments_end = std::end(std::forward<decltype(arguments)>(arguments));

        auto argument_strings_i = begin();
        const auto argument_strings_end = end();

        for (; arguments_i != arguments_end &&
               argument_strings_i != argument_strings_end;
             ++arguments_i, ++argument_strings_i)
        {
            argument_& argument = *arguments_i;

            const auto success_local = argument.parse(*argument_strings_i);

            if (success_local)
            {
                argument.set_presence();
            }
            else
            {
                success = false;
            }
        }

        return success;
    }
};
}
