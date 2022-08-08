#include <PPsat/subprogram/dpll.hpp>

#include <PPsat/builder.hpp>
#include <PPsat/builder_DIMACS.hpp>
#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/conditional.hpp>
#include <PPsat/create_builder.hpp>
#include <PPsat/discard_iterator.hpp>
#include <PPsat/error_listener.hpp>
#include <PPsat/factory.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/formula_simple.hpp>
#include <PPsat/logger_subroutine.hpp>
#include <PPsat/renaming_map.hpp>

#include <iostream>
#include <variant>

namespace
{
int dpll(const PPsat::logger& logger_inner,
         std::istream& input,
         std::ostream& output,
         const PPsat::builder& builder)
{
    PPsat::formula_simple formula;
    const auto renaming = builder.create();

    const auto result = builder.read(logger_inner, input, formula, *renaming);

    if (!result)
    {
        logger_inner << "Input parsing failed.\n";

        return 1;
    }

    return 0;
}
}

PPsat::subcommand_result PPsat::subprogram::dpll_unparsed(
    const logger& logger_outer,
    cli::arguments& arguments,
    options& options)
{
    if (!options.dpll)
    {
        return {};
    }

    const auto& logger_inner = logger_subroutine(logger_outer, "dpll");

    PPsat::cli::argument::file_in argument_file_in;

    const auto success =
        arguments.parse(logger_inner, std::array{std::ref(argument_file_in)});

    if (!success)
    {
        return 1;
    }

    return dpll(logger_inner,
                argument_file_in.parsed_stream(),
                std::cout,
                create_builder(options.format, argument_file_in, options.nnf))
           << 1;
}
