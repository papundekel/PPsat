#include <PPsat/cli/options.hpp>
#include <PPsat/subprogram/cdcl.hpp>
#include <PPsat/subprogram/convert.hpp>
#include <PPsat/subprogram/dpll.hpp>
#include <PPsat/subprogram/help.hpp>

#include <PPsat-base/cli/arguments_container.hpp>
#include <PPsat-base/cli/error_handler_simple.hpp>
#include <PPsat-base/cli/parser.hpp>
#include <PPsat-base/containers.hpp>
#include <PPsat-base/logger_ostream.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <array>
#include <iostream>
#include <iterator>
#include <string_view>
#include <vector>

int main(int argc, char** argv)
{
    const auto logger_cerr = PPsat_base::logger_ostream(std::cerr);
    const auto logger = PPsat_base::logger_subroutine(logger_cerr, "PPsat");

    PPsat::cli::options options;
    PPsat_base::cli::arguments_container<PPsat_base::vector> arguments;
    PPsat_base::cli::error_handler_simple error_handler(std::cerr);

    const auto parse_success = PPsat_base::cli::parser(options.as_range())
                                   .parse(argc, argv, arguments, error_handler);

    if (!parse_success)
    {
        error_handler.handle();

        return 1;
    }

    constexpr auto subprograms = std::array{PPsat::subprogram::help_unparsed,
                                            PPsat::subprogram::convert_unparsed,
                                            PPsat::subprogram::cdcl_unparsed,
                                            PPsat::subprogram::dpll_unparsed};

    int sub_code;
    const auto i =
        std::find_if(std::begin(subprograms),
                     std::end(subprograms),
                     [&arguments, &options, &sub_code, &logger](auto subprogram)
                     {
                         auto result = subprogram(logger, arguments, options);
                         if (result)
                         {
                             sub_code = result.code();
                             return true;
                         }

                         return false;
                     });

    if (i == std::end(subprograms))
    {
        logger << "No subprogram specified.\n";
        PPsat::subprogram::help_print(std::cout);
        return 2;
    }
    else
    {
        return sub_code << 2;
    }
}
