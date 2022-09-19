#include <PPsat-crypt/visitor_constraints.hpp>
#include <PPsat-crypt/visitor_variables.hpp>

#include <PPsat-crypt-lexer/lexer_CRYPT.h>
#include <PPsat-crypt-parser/parser_CRYPT.h>

#include <PPsat-base/antlrer.hpp>
#include <PPsat-base/cli/argument.hpp>
#include <PPsat-base/cli/argument/simple_number.hpp>
#include <PPsat-base/cli/option.hpp>
#include <PPsat-base/cli/option/simple_named_bool.hpp>
#include <PPsat-base/cli/option/simple_named_int.hpp>
#include <PPsat-base/cli/parser.hpp>
#include <PPsat-base/logger_ostream.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <ostream>
#include <ranges>
#include <sstream>
#include <type_traits>

namespace
{
void help_print(std::ostream& output)
{
    output << "Description:\n"
           << "\tGenerates an SMT-LIB formula from an input cryptarithm file. "
              "The input is accepted from stdin.\n"
           << "\n"
           << "Usage:\n"
           << "\tPPsat-crypt-generate [OPTION]...\n"
           << "\n"
           << "Options:\n"
           << "\t-help [<BOOL>]\tDisplays this help.\n"
           << "\t-base <UINT>\tThe base. The value is 10 if unspecified.\n"
           << "\t-repeating\tAllows multiple characters to represent the "
           << "same value\n"
           << "\n"
           << "Examples:\n"
           << "\tPPsat-crypt-generate -repeating < input\n";
}

class parameters_value
{
public:
    bool help;
    std::size_t base;
    bool repeating;
};

class parameters final : public PPsat_base::cli::parameters<parameters_value>
{
    PPsat_base::cli::option::simple_named_bool help;
    PPsat_base::cli::option::simple_named_int base;
    PPsat_base::cli::option::simple_named_bool repeating;

public:
    parameters()
        : help(false, "help", false)
        , base(false, "base", 10)
        , repeating(false, "repeating", false)
    {}

private:
    void options(
        std::function<void(PPsat_base::cli::option_&)> f) override final
    {
        f(help);
        f(base);
        f(repeating);
    }

    void arguments(
        std::function<void(PPsat_base::cli::argument_&)> f) override final
    {}

public:
    parameters_value parsed() override final
    {
        return parameters_value{help.parsed(),
                                base.parsed(),
                                repeating.parsed()};
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
        PPsat_base::logger_subroutine(logger_cerr, "PPsat-crypt-generate");

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

    antlr4::ANTLRInputStream input(cin);

    PPsat_base::antlrer antlrer(
        logger,
        input,
        PPsat_base::factory_lexer::impl<PPsat_crypt::lexer_CRYPT>(),
        PPsat_base::factory_parser::impl<PPsat_crypt::parser_CRYPT>(),
        true);

    auto* const parse_tree = antlrer.parse();

    if (!parse_tree)
    {
        logger << "Skipping outputting the formula.\n";
        return 2;
    }

    const auto base = parameters.base;

    PPsat_crypt::visitor_variables visitor(base, !parameters.repeating, cout);
    visitor.visit(parse_tree);
    PPsat_crypt::visitor_constraints(base, cout).visit(parse_tree);
    visitor.print_get_value();

    return 0;
}
}
