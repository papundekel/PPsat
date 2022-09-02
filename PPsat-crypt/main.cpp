#include <PPsat-crypt/visitor_constraints.hpp>
#include <PPsat-crypt/visitor_variables.hpp>

#include <PPsat-crypt-lexer/lexer_CRYPT.h>
#include <PPsat-crypt-parser/parser_CRYPT.h>

#include <PPsat-base/antlrer.hpp>
#include <PPsat-base/cli/argument.hpp>
#include <PPsat-base/cli/argument/simple_number.hpp>
#include <PPsat-base/cli/option.hpp>
#include <PPsat-base/cli/option/simple_named_bool.hpp>
#include <PPsat-base/cli/option/simple_named_int.hpp>
#include <PPsat-base/cli/parser.hpp>
#include <PPsat-base/logger_ostream.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <ostream>
#include <ranges>
#include <sstream>
#include <type_traits>

namespace
{
void help_print(std::ostream& output)
{
    output << "Description:\n"
           << "\tGenerates an SMT-LIB formula from an input cryptarithm file. "
              "The input is accepted from stdin.\n"
           << "\n"
           << "Usage:\n"
           << "\tPPsat-crypt-generate [OPTION]...\n"
           << "\n"
           << "Options:\n"
           << "\t-help [<BOOL>]\tDisplays this help.\n"
           << "\t-base <UINT>\tThe base. The value is 10 if unspecified.\n"
           << "\t-repeating\tAllows multiple characters to represent the "
           << "same value\n"
           << "\n"
           << "Examples:\n"
           << "\tPPsat-crypt-generate -repeating < inputs/basic.crypt\n";
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
        PPsat_base::logger_subroutine(logger_cerr, "PPsat-crypt-generate");

    PPsat_base::cli::option::simple_named_bool option_help("help");
    PPsat_base::cli::option::simple_named_int option_base("base", 10);
    PPsat_base::cli::option::simple_named_bool option_repeating("repeating");

    const auto success_cli =
        PPsat_base::cli::parser(
            std::array{
                std::reference_wrapper<PPsat_base::cli::option_>(option_help),
                std::reference_wrapper<PPsat_base::cli::option_>(option_base),
                std::reference_wrapper<PPsat_base::cli::option_>(
                    option_repeating)},
            std::views::empty<PPsat_base::cli::argument_>)
            .parse(argc, argv, logger);

    if (!success_cli)
    {
        return 1;
    }

    if (option_help.parsed())
    {
        help_print(cout);
        return 0;
    }

    antlr4::ANTLRInputStream input(cin);

    PPsat_base::antlrer antlrer(
        logger,
        input,
        PPsat_base::factory_lexer::impl<PPsat_crypt::lexer_CRYPT>(),
        PPsat_base::factory_parser::impl<PPsat_crypt::parser_CRYPT>(),
        true);

    auto* const parse_tree = antlrer.parse();

    if (!parse_tree)
    {
        logger << "Skipping outputting the formula.\n";
        return 2;
    }

    const auto base = option_base.parsed();

    PPsat_crypt::visitor_variables visitor(base,
                                           !option_repeating.parsed(),
                                           cout);
    visitor.visit(parse_tree);
    PPsat_crypt::visitor_constraints(base, cout).visit(parse_tree);
    visitor.print_get_value();

    return 0;
}
}
