#include "PPsat/logger_subroutine.hpp"
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/discard_iterator.hpp>
#include <PPsat/subprogram/cdcl.hpp>

#include <array>
#include <iostream>

namespace
{
int cdcl(std::istream& input,
         std::ostream& output,
         const PPsat::logger& logger_outer,
         const PPsat::formula_format format,
         const bool watched_literals,
         const bool nnf)
{
    return 0;
}
}

PPsat::subcommand_result PPsat::subprogram::cdcl_unparsed(
    const logger& logger_outer,
    cli::arguments& arguments,
    options& options)
{
    if (!options.cdcl)
    {
        return {};
    }

    const auto& logger_inner = logger_subroutine(logger_outer, "cdcl");

    PPsat::cli::argument::file_in argument_in;

    const auto success =
        arguments.parse(logger_inner, std::array{std::ref(argument_in)});

    if (!success)
    {
        logger_inner << "Skipping the algorithm.\n";
        return 1;
    }

    return cdcl(argument_in.parsed_stream(),
                std::cout,
                logger_inner,
                options.format ? options.format.parsed_format()
                : argument_in  ? argument_in.parsed_format()
                               : formula_format::DIMACS,
                options.watched_literals,
                options.nnf);
}
