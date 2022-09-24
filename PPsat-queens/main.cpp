#include <PPsat-base/cli/parameters.hpp>
#include <PPsat-queens/generate.hpp>

#include <PPsat-base/cli/argument/number.hpp>
#include <PPsat-base/cli/argument/simple_number.hpp>
#include <PPsat-base/cli/option/simple_named_bool.hpp>
#include <PPsat-base/cli/parser.hpp>
#include <PPsat-base/logger_ostream.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <array>
#include <charconv>
#include <cstring>
#include <iostream>
#include <ranges>

namespace
{
void help_print(std::ostream& output)
{
    output << "Description:\n"
           << "\tGenerates a CNF DIMACS formula representing the N queens "
              "puzzle. Uses stdout.\n"
           << "\n"
           << "Usage:\n"
           << "\tPPsat-queens-generate <dimension>\n"
           << "\tPPsat-queens-generate -help\n"
           << "\n"
           << "Arguments:\n"
           << "\tdimension\tThe dimension of the chess board.\n"
           << "\n"
           << "Examples:\n"
           << "\tPPsat-queens-generate 8\n";
}

class parameters_value
{
public:
    bool help;
    std::size_t dimension;
};

class parameters final : public PPsat_base::cli::parameters<parameters_value>
{
    PPsat_base::cli::option::simple_named_bool help;
    PPsat_base::cli::argument::simple_number dimension;

public:
    parameters()
        : help(false, "help", false)
        , dimension(true)
    {}

private:
    void options(
        std::function<void(PPsat_base::cli::option_&)> f) override final
    {
        f(help);
    }

    void arguments(
        std::function<void(PPsat_base::cli::argument_&)> f) override final
    {
        f(dimension);
    }

public:
    parameters_value parsed() override final
    {
        return parameters_value{help.parsed(), dimension.parsed()};
    }
};
}

namespace PPexe
{
int main(std::istream& cin,
         std::ostream& cout,
         std::ostream& cerr,
         int argc,
         char** argv)
{
    const auto logger_cerr = PPsat_base::logger_ostream(cerr);
    const auto logger =
        PPsat_base::logger_subroutine(logger_cerr, "PPsat-queens::generate");

    parameters parameters_declaration;

    const auto success_cli = PPsat_base::cli::parser(parameters_declaration)
                                 .parse(argc, argv, logger);

    if (!success_cli)
    {
        return 1;
    }

    const auto parameters = parameters_declaration.parsed();

    if (parameters.help)
    {
        help_print(cout);
        return 0;
    }

    PPsat_queens::generate(parameters.dimension, cout);

    return 0;
}
}
