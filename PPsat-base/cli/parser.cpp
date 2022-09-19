#include "PPsat-base/cli/option.hpp"
#include <PPsat-base/cli/parser.hpp>

#include <PPsat-base/logger_subroutine.hpp>

#include <algorithm>
#include <iostream>
#include <ranges>

decltype(PPsat_base::cli::parser::options)::const_iterator
PPsat_base::cli::parser::find(const std::string_view name) const
{
    const auto i = std::ranges::lower_bound(options,
                                            name,
                                            std::ranges::less(),
                                            &key_value_pair::first);

    if (i != options.end() && i->first == name)
        return i;
    else
        return options.end();
}

bool PPsat_base::cli::parser::parse(const int argc,
                                    char** const argv,
                                    const logger& logger_outer) const
{
    const auto logger = logger_subroutine(logger_outer, "cli_parse");

    auto success = true;

    const auto end = argv + argc;

    auto argument_i = arguments.begin();

    for (char** i = argv + 1; i != end;)
    {
        const auto argument_string = std::string_view(*i++);

        if (argument_string[0] == '-')
        {
            const auto option_string = argument_string.substr(1);

            const auto var_i = find(option_string);

            if (var_i == options.end())
            {
                logger << "Invalid option: \"" << option_string << "\"\n";
                return false;
            }

            option_& option = var_i->second;

            option.set_presence();

            const auto option_argument_count_min = option.argument_count_min();
            const auto option_argument_count_max = option.argument_count_max();
            auto option_argument_counter = 0uz;

            for (; option_argument_counter != option_argument_count_max &&
                   i != end;
                 ++option_argument_counter, ++i)
            {
                const auto option_argument = *i;

                const auto option_argument_parse_success =
                    option.parse(logger,
                                 option_argument_counter,
                                 option_argument);

                if (!option_argument_parse_success)
                {
                    if (option_argument_counter < option_argument_count_min)
                    {
                        logger << "Option `" << option_string
                               << "`'s argument \"" << option_argument
                               << "\" parsing failed.\n";
                    }
                    break;
                }
            }

            if (option_argument_counter < option_argument_count_min)
            {
                logger << "Not enough `" << option_string
                       << "` option arguments, should be at least"
                       << option_argument_count_min << ".\n ";
                return false;
            }
        }
        else
        {
            if (argument_i == arguments.end())
            {
                logger << "Too many arguments.\n";
                return false;
            }

            argument_& argument = *argument_i++;

            const auto success = argument.parse(logger, argument_string);

            if (!success)
            {
                logger << "Argument parsing failed.\n";
                return false;
            }

            argument.set_presence();
        }
    }

    for (auto& [_, option_ref] : options)
    {
        option_& option = option_ref;

        if (option.is_required() && !option.is_present())
        {
            logger << "Option `" << option.name() << "` is required.\n";
            return false;
        }
    }

    return true;
}
