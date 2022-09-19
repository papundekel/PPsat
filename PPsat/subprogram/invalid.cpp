#include <PPsat/subprogram.hpp>

int PPsat::subprogram::invalid(const PPsat_base::logger& logger_outer,
                               const cli::parameters_value&)
{
    logger_outer << "Invalid subprogram.\n";

    return 1;
}
