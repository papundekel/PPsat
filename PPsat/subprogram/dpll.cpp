#include <PPsat/cli/argument/file.hpp>
#include <PPsat/discard_iterator.hpp>
#include <PPsat/error_listener_simple_detect.hpp>
#include <PPsat/subprogram/dpll.hpp>

#include <PPsat-lexer_DIMACS/lexer_DIMACS.h>
#include <PPsat-lexer_SMTLIB/lexer_SMTLIB.h>
#include <PPsat-parser_DIMACS/parser_DIMACS.h>
#include <PPsat-parser_SMTLIB/parser_SMTLIB.h>

#include <iostream>
#include <variant>

PPsat::subcommand_result PPsat::subprogram::dpll_unparsed(
    cli::arguments& arguments,
    options& options)
{
    if (!options.dpll)
    {
        return {};
    }

    PPsat::cli::argument::file_in argument_in;

    arguments.parse(std::ranges::views::single(std::ref(argument_in)));

    return dpll(argument_in.parsed_stream(),
                std::cout,
                std::cerr,
                options.format ? options.format.parsed_format()
                : argument_in  ? argument_in.parsed_format()
                               : formula_format::DIMACS,
                options.watched_literals,
                options.nnf);
}

int PPsat::subprogram::dpll(std::istream& input,
                            std::ostream& output,
                            std::ostream& err,
                            formula_format format,
                            bool watched_literals,
                            bool nnf)
{
    // parser.parse(input, nnf)

    return 0;
}
