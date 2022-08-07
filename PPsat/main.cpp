#include <PPsat/cli/arguments_container.hpp>
#include <PPsat/cli/option/parser.hpp>
#include <PPsat/error_handler.hpp>
#include <PPsat/options.hpp>
#include <PPsat/subprogram/cdcl.hpp>
#include <PPsat/subprogram/convert.hpp>
#include <PPsat/subprogram/dpll.hpp>
#include <PPsat/subprogram/help.hpp>
#include <PPsat/vector.hpp>

#include <array>
#include <iostream>
#include <iterator>
#include <string_view>
#include <vector>

int main(int argc, char** argv)
{
    PPsat::options options;
    PPsat::cli::arguments_container<PPsat::vector> arguments;
    PPsat::error_handler error_handler(std::cerr);

    const auto parse_success = PPsat::cli::option::parser(options.as_range())
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
                     [&arguments, &options, &sub_code](auto subprogram)
                     {
                         auto result = subprogram(arguments, options);
                         if (result)
                         {
                             sub_code = result.code();
                             return true;
                         }

                         return false;
                     });

    if (i == std::end(subprograms))
    {
        std::cerr << "No subprogram specified.\n";
        return 2;
    }
    else
    {
        return sub_code << 2;
    }
}
