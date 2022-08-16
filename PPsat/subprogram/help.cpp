#include <PPsat/subprogram/help.hpp>

#include <iostream>

void PPsat::subprogram::help_print(std::ostream& out)
{
    out << "Usage:\n"
        << "\tPPsat -convert [OPTION]... [INPUT [OUTPUT]]\n"
        << "\tPPsat -cdcl [OPTION]... [INPUT]\n"
        << "\tPPsat -dpll [OPTION]... [INPUT]\n"
        << "\tPPsat -help\n"
        << "\n"
        << "\tMissing file name means the program will use standard I/O.\n"
        << "\n"
        << "Options:\n"
        << "\t-nnf\t\t\tThe input is in NNF.\n"
        << "\t-wl\t\t\tUse watched literals.\n"
        << "\t-format [dimacs|smtlib]\tUse the specified format for the "
           "input.\n";
}

PPsat::subcommand_result PPsat::subprogram::help_unparsed(
    const PPsat_base::logger&,
    PPsat_base::cli::arguments&,
    cli::options& options)
{
    if (!options.help)
    {
        return {};
    }

    help_print(std::cout);

    return 0;
}
