#include <PPsat/cli/argument/file.hpp>
#include <PPsat/discard_iterator.hpp>
#include <PPsat/subprogram/cdcl.hpp>

#include <array>
#include <iostream>

PPsat::subcommand_result PPsat::subprogram::cdcl_unparsed(
    cli::arguments& arguments,
    options& options)
{
    if (!options.cdcl)
    {
        return {};
    }

    PPsat::cli::argument::file_in argument_in;

    arguments.parse(std::array{std::ref(argument_in)});

    return cdcl(argument_in.parsed_stream(),
                std::cout,
                std::cerr,
                options.format ? options.format.parsed_format()
                : argument_in  ? argument_in.parsed_format()
                               : formula_format::DIMACS,
                options.watched_literals,
                options.nnf);
}

int PPsat::subprogram::cdcl(std::istream& input,
                            std::ostream& output,
                            std::ostream& err,
                            const formula_format format,
                            const bool watched_literals,
                            const bool nnf)
{
    return 0;
}
