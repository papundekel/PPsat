#include <PPsat/help_print.hpp>
#include <PPsat/subprogram.hpp>

#include <iostream>

int PPsat::subprogram::help(const PPsat_base::logger&,
                            cli::options&,
                            cli::argument::file_in&,
                            cli::argument::file_out&)
{
    help_print(std::cout);

    return 0;
}
