#include <PPsat-crypt/visitor_constraints.hpp>
#include <PPsat-crypt/visitor_variables.hpp>

#include <PPsat-crypt-lexer/lexer_CRYPT.h>
#include <PPsat-crypt-parser/parser_CRYPT.h>

#include <PPsat-base/antlrer.hpp>
#include <PPsat-base/cli/argument.hpp>
#include <PPsat-base/cli/argument/simple_number.hpp>
#include <PPsat-base/cli/option.hpp>
#include <PPsat-base/cli/option/simple_named_bool.hpp>
#include <PPsat-base/cli/option/simple_named_number.hpp>
#include <PPsat-base/cli/parser.hpp>
#include <PPsat-base/containers.hpp>
#include <PPsat-base/formula.hpp>
#include <PPsat-base/logger_ostream.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <functional>
#include <iostream>
#include <iterator>
#include <ranges>
#include <type_traits>

namespace
{
void help_print(std::ostream& output)
{
    output << "Usage:\n"
           << "\tPPsat-crypt <BASE>\n"
           << "\tPPsat-crypt -help\n";
}
}

int main(int argc, char** argv)
{
    const auto logger_cerr = PPsat_base::logger_ostream(std::cerr);
    const auto logger =
        PPsat_base::logger_subroutine(logger_cerr, "PPsat-crypt");

    PPsat_base::cli::option::simple_named_bool option_help("help");
    PPsat_base::cli::option::simple_named_number option_base("base");
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

    if (option_help)
    {
        help_print(std::cout);
        return 0;
    }

    PPsat_base::antlrer antlrer(
        logger,
        std::cin,
        PPsat_base::factory_lexer::impl<PPsat_crypt::lexer_CRYPT>(),
        PPsat_base::factory_parser::impl<PPsat_crypt::parser_CRYPT>());

    auto* const parse_tree = antlrer.parse();

    if (!parse_tree)
    {
        logger << "Skipping outputting the formula.\n";
        return 2;
    }

    const auto base = option_base ? option_base.parsed() : 10uz;

    PPsat_crypt::visitor_variables visitor(base, !option_repeating, std::cout);
    visitor.visit(parse_tree);
    PPsat_crypt::visitor_constraints(base, std::cout).visit(parse_tree);
    visitor.print_get_value();

    return 0;
}
