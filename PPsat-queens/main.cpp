#include <PPsat-base/cli/argument/simple_number.hpp>
#include <PPsat-base/cli/option/simple_named_bool.hpp>
#include <PPsat-queens/generate.hpp>

#include <PPsat-base/cli/argument/number.hpp>
#include <PPsat-base/cli/parser.hpp>
#include <PPsat-base/logger_ostream.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <array>
#include <charconv>
#include <cstring>
#include <iostream>
#include <ranges>

namespace
{
void help_print(std::ostream& output)
{
    output << "Usage:\n"
           << "\tPPsat-queens-generate <DIMENSION>\n"
           << "\tPPsat-queens-generate -help\n";
}
}

int main(int argc, char** argv)
{
    const auto logger_cerr = PPsat_base::logger_ostream(std::cerr);
    const auto logger =
        PPsat_base::logger_subroutine(logger_cerr, "PPsat-queens");

    PPsat_base::cli::option::simple_named_bool help("help");
    PPsat_base::cli::argument::simple_number argument_dimension;

    const auto success_cli =
        PPsat_base::cli::parser(
            std::views::single(std::ref(help)),
            std::views::single(std::ref(argument_dimension)))
            .parse(argc, argv, logger);

    if (!success_cli)
    {
        return 1;
    }

    if (help)
    {
        help_print(std::cout);
        return 0;
    }

    if (!argument_dimension)
    {
        logger << "No dimension argument provided.\n";
        return 2;
    }

    PPsat_queens::generate(argument_dimension.parsed(), std::cout);

    return 0;
}
