#pragma once
#include <PPsat/cli/argument.hpp>
#include <PPsat/cli/option.hpp>
#include <PPsat/cli/error_handler.hpp>
#include <PPsat/cli/arguments.hpp>

#include <algorithm>
#include <ranges>
#include <string_view>
#include <vector>

namespace PPsat::cli::option
{
class parser
{
    using key_value_pair =
        std::pair<std::string_view, std::reference_wrapper<option_>>;

    std::vector<key_value_pair> option_map;

    decltype(option_map)::const_iterator find(std::string_view name) const;

public:
    parser(auto&& options)
    {
        std::ranges::copy(
            std::forward<decltype(options)>(options) |
                std::views::transform(
                    [](option_& option)
                    {
                        return key_value_pair{option.name(), option};
                    }),
            std::back_inserter(option_map));

        std::ranges::sort(option_map,
                          [](const auto& a, const auto& b)
                          {
                              return a.first < b.first;
                          });
    }

    bool parse(int argc,
               char** argv,
               arguments& arguments,
               error_handler& error_handler) const
    {
        auto success = true;

        const auto end = argv + argc;

        for (char** i = argv + 1; i != end;)
        {
            const auto cl_argument = std::string_view(*i++);

            if (cl_argument[0] == '-')
            {
                const auto option = cl_argument.substr(1);

                const auto var_i = find(option);

                if (var_i != option_map.end())
                {
                    option_& o = var_i->second;

                    o.set_presence();

                    const auto arg_count = o.argument_count();
                    auto arg_counter = 0z;

                    for (; arg_counter != arg_count && i != end;
                         ++arg_counter, ++i)
                    {
                        const auto argument = *i;

                        const auto option_argument_parse_success =
                            o.parse(arg_counter, argument);

                        if (!option_argument_parse_success)
                        {
                            success = false;

                            error_handler.unrecognized_option_argument(argument);
                        }
                    }

                    if (arg_counter != arg_count)
                    {
                        success = false;
                    }
                }
                else
                {
                    success = false;

                    error_handler.unrecognized_argument(option);
                }
            }
            else
            {
                arguments.add(cl_argument);
            }
        }

        return success;
    }
};
}
