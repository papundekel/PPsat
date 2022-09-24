#include <PPsat/cli/argument/file.hpp>
#include <PPsat/cli/parameters.hpp>
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

    PPsat::cli::parameters parameters;

    const auto success_cli =
        PPsat_base::cli::parser(parameters).parse(argc, argv, logger);

    if (!success_cli)
    {
        logger << "CLI error, stopping.\n";
        return 1;
    }

    const auto parameters_value = parameters.parsed();

    if (parameters_value.help)
    {
        PPsat::help_print(cout);
        return 0;
    }

    return [](PPsat::subprogram::selection subprogram)
    {
        switch (subprogram)
        {
            case PPsat::subprogram::selection::convert:
                return PPsat::subprogram::convert;
            case PPsat::subprogram::selection::solve:
                return PPsat::subprogram::solve;
            default:
                return PPsat::subprogram::invalid;
        }
    }(parameters_value.subprogram)(logger, parameters_value)
           << 1;
}
}
