#include <PPsat/cli/argument/file.hpp>
#include <PPsat/discard_iterator.hpp>
#include <PPsat/subprogram/dpll.hpp>

#include <iostream>

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
    output << "dpll\n";
    output << (int)format << "\n";
    output << watched_literals << "\n";
    output << nnf << "\n";
    return 0;
}
