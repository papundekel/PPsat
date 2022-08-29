#include <PPsat/cli/argument/file.hpp>
#include <PPsat/cli/options.hpp>
#include <PPsat/help_print.hpp>
#include <PPsat/subprogram.hpp>

#include <PPsat-base/cli/argument.hpp>
#include <PPsat-base/cli/parser.hpp>
#include <PPsat-base/logger_ostream.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <array>
#include <functional>
#include <iostream>
#include <iterator>
#include <string_view>
#include <vector>

namespace PPexe
{
int main(std::istream& cin,
         std::ostream& cout,
         std::ostream& cerr,
         int argc,
         char** argv)
{
    const auto logger_cerr = PPsat_base::logger_ostream(cerr);
    const auto logger = PPsat_base::logger_subroutine(logger_cerr, "PPsat");

    PPsat::cli::options options;
    PPsat::cli::argument::file_in argument_file_in;
    PPsat::cli::argument::file_out argument_file_out;

    const auto success_cli =
        PPsat_base::cli::parser(
            options.as_range(),
            std::array{std::reference_wrapper<PPsat_base::cli::argument_>(
                           argument_file_in),
                       std::reference_wrapper<PPsat_base::cli::argument_>(
                           argument_file_out)})
            .parse(argc, argv, logger);

    if (!success_cli)
    {
        logger << "CLI error, stopping.\n";
        return 1;
    }

    if (options["help"_cst].parsed())
    {
        PPsat::help_print(cout);
        return 0;
    }

    return options["subprogram"_cst].parsed_subprogram()(logger,
                                                         options,
                                                         argument_file_in,
                                                         argument_file_out)
           << 1;
}
}
