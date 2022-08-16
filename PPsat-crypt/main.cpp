#include <PPsat-crypt/clause.hpp>
#include <PPsat-crypt/cli/options.hpp>
#include <PPsat-crypt/variable.hpp>
#include <PPsat-crypt/visitor_constraints.hpp>
#include <PPsat-crypt/visitor_variable_def.hpp>

#include <PPsat-crypt-lexer_CRYPT/lexer_CRYPT.h>
#include <PPsat-crypt-parser_CRYPT/parser_CRYPT.h>

#include <PPsat-base/antlrer.hpp>
#include <PPsat-base/cli/argument/simple_number.hpp>
#include <PPsat-base/cli/arguments_container.hpp>
#include <PPsat-base/cli/error_handler_simple.hpp>
#include <PPsat-base/cli/option/simple_named_bool.hpp>
#include <PPsat-base/cli/parser.hpp>
#include <PPsat-base/containers.hpp>
#include <PPsat-base/formula.hpp>
#include <PPsat-base/logger_ostream.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <iostream>
#include <iterator>
#include <ranges>

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

    PPsat_base::cli::arguments_container<PPsat_base::vector> arguments;
    PPsat_base::cli::error_handler_simple error_handler(std::cerr);

    PPsat_base::cli::option::simple_named_bool help("help");

    const auto success_cli_options =
        PPsat_base::cli::parser(std::views::single(std::ref(help)))
            .parse(argc, argv, arguments, error_handler);

    PPsat_base::cli::argument::simple_number argument_base;

    const auto success_cli =
        success_cli_options &&
        arguments.parse(logger, std::array{std::ref(argument_base)});

    if (!success_cli)
    {
        error_handler.handle();

        return 1;
    }

    if (help)
    {
        help_print(std::cout);
        return 0;
    }

    if (!argument_base)
    {
        logger << "No base argument provided.\n";

        return 1;
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
        return 1;
    }

    const auto base = argument_base.parsed();

    PPsat_crypt::visitor_variable_def(base, std::cout).visit(parse_tree);
    PPsat_crypt::visitor_constraints(base, std::cout).visit(parse_tree);

    return 0;
}
