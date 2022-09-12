#include <PPsat-queens/generate.hpp>

#include <PPsat-base/cli/argument/number.hpp>
#include <PPsat-base/cli/argument/simple_number.hpp>
#include <PPsat-base/cli/option/simple_named_bool.hpp>
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
    output << "Description:\n"
           << "\tGenerates a CNF DIMACS formula representing the N queens "
              "puzzle. Uses stdout.\n"
           << "\n"
           << "Usage:\n"
           << "\tPPsat-queens-generate <dimension>\n"
           << "\tPPsat-queens-generate -help\n"
           << "\n"
           << "Arguments:\n"
           << "\tdimension\tThe dimension of the chess board.\n"
           << "\n"
           << "Examples:\n"
           << "\tPPsat-queens-generate 8\n";
}
}

namespace PPexe
{
int main(std::istream& cin,
         std::ostream& cout,
         std::ostream& cerr,
         int argc,
         char** argv)
{
    const auto logger_cerr = PPsat_base::logger_ostream(cerr);
    const auto logger =
        PPsat_base::logger_subroutine(logger_cerr, "PPsat-queens::generate");

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

    if (help.parsed())
    {
        help_print(cout);
        return 0;
    }

    if (!argument_dimension.is_present())
    {
        logger << "No dimension argument provided.\n";
        return 2;
    }

    PPsat_queens::generate(argument_dimension.parsed(), cout);

    return 0;
}
}
