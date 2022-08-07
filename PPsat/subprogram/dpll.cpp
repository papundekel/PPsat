#include <PPsat/subprogram/dpll.hpp>

#include "PPsat-parser_DIMACS/parser_DIMACS.h"
#include "PPsat/factory.hpp"
#include "PPsat/formula_format.hpp"
#include "PPsat/reader_SMTLIB_tseitin.hpp"
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/conditional.hpp>
#include <PPsat/discard_iterator.hpp>
#include <PPsat/error_listener_simple_detect.hpp>
#include <PPsat/formula_simple.hpp>
#include <PPsat/reader.hpp>
#include <PPsat/reader_DIMACS.hpp>
#include <PPsat/renaming_map.hpp>

#include <iostream>
#include <variant>

namespace
{
int dpll(std::istream& input,
         std::ostream& output,
         std::ostream& err,
         const PPsat::reader& reader)
{
    PPsat::formula_simple formula;
    const auto renaming = reader.create();

    const auto result = reader.read(input, formula, *renaming);

    if (!result)
    {
        return 1;
    }

    return 0;
}

auto pick_format(PPsat::options& options, PPsat::cli::argument::file_in& file)
{
    if (options.format)
    {
        return options.format.parsed_format();
    }
    else if (file.is_present())
    {
        return file.parsed_format();
    }

    return PPsat::formula_format::DIMACS;
}
}

PPsat::subcommand_result PPsat::subprogram::dpll_unparsed(
    cli::arguments& arguments,
    options& options)
{
    if (!options.dpll)
    {
        return {};
    }

    PPsat::cli::argument::file_in argument_file_in;

    arguments.parse(std::array{std::ref(argument_file_in)});

    return dpll(argument_file_in.parsed_stream(),
                std::cout,
                std::cerr,
                conditional<reader>(pick_format(options, argument_file_in) ==
                                        formula_format::DIMACS,
                                    reader_DIMACS{},
                                    reader_SMTLIB_tseitin(options.nnf)));
}
