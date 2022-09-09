#pragma once
#include <PPsat-base/cli/argument.hpp>
#include <PPsat-base/cli/option.hpp>
#include <PPsat-base/ranges_to.hpp>

#include <algorithm>
#include <functional>
#include <ranges>
#include <string_view>
#include <vector>

namespace PPsat_base::cli
{
class parser
{
    using key_value_pair =
        std::pair<std::string_view, std::reference_wrapper<option_>>;

    std::vector<key_value_pair> options;
    std::vector<std::reference_wrapper<argument_>> arguments;

    decltype(options)::const_iterator find(std::string_view name) const;

public:
    parser(auto&& options, auto&& arguments)
    // : options(PPsat_base::ranges_to<std::vector>(
    //       std::forward<decltype(options)>(options) |
    //       std::views::transform(
    //           [](option_& option)
    //           {
    //               return key_value_pair{option.name(), option};
    //           })))
    // , arguments(PPsat_base::ranges_to<std::vector>(
    //       std::forward<decltype(arguments)>(arguments) |
    //       std::views::transform(
    //           [](argument_& argument)
    //           {
    //               return std::ref(argument);
    //           })))
    {
        for (option_& option : std::forward<decltype(options)>(options))
        {
            this->options.emplace_back(option.name(), option);
        }

        for (argument_& argument : std::forward<decltype(arguments)>(arguments))
        {
            this->arguments.emplace_back(argument);
        }

        std::ranges::sort(this->options,
                          [](const auto& a, const auto& b)
                          {
                              return a.first < b.first;
                          });
    }

    bool parse(const int argc,
               char** const argv,
               const logger& logger_outer) const;
};
}
