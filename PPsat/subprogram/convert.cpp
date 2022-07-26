#include <PPsat/CNF.hpp>
#include <PPsat/builder_tseitin.hpp>
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/discard_iterator.hpp>
#include <PPsat/error_listener_simple_detect.hpp>
#include <PPsat/literal_pair.hpp>
#include <PPsat/reader_SMTLIB_tseitin.hpp>
#include <PPsat/small_static_storage.hpp>
#include <PPsat/subprogram/convert.hpp>

#include <PPsat-lexer_SMTLIB/lexer_SMTLIB.h>
#include <PPsat-parser_SMTLIB/parser_SMTLIB.h>

#include <antlr4-runtime.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

namespace
{
constexpr auto greater_power_10(auto x) noexcept
{
    auto power = 10;

    while (x >= power)
    {
        power *= 10;
    }

    return power;
}

template <typename T>
using clause_storage = PPsat::small_static_storage<T, 3>;
}

PPsat::subcommand_result PPsat::subprogram::convert_unparsed(
    cli::arguments& arguments,
    options& options)
{
    if (!options.convert)
    {
        return {};
    }

    PPsat::cli::argument::file_in argument_in;
    PPsat::cli::argument::file_out argument_out;

    bool success = arguments.parse(
        std::array<std::reference_wrapper<PPsat::cli::argument_>, 2>{
            argument_in,
            argument_out});

    return convert(argument_in.parsed_stream(),
                   argument_out.parsed_stream(),
                   std::cerr,
                   options.nnf);
}

int PPsat::subprogram::convert(std::istream& input,
                               std::ostream& output,
                               std::ostream& err,
                               bool nnf)
{
    error_listener_simple_detect error_listener;

    antlr4::ANTLRInputStream input_antlr(input);

    lexer_SMTLIB lexer(&input_antlr);
    lexer.addErrorListener(&error_listener);

    antlr4::CommonTokenStream tokens(&lexer);

    parser_SMTLIB parser(&tokens);
    parser.addErrorListener(&error_listener);

    auto* const input_parsed = parser.input();

    if (error_listener.error_encountered())
    {
        err << "Parse error encountered, quitting.\n";
        return 1;
    }

    std::map<std::string, std::size_t> renaming;
    CNF<std::vector, clause_storage, literal_pair<std::size_t>> formula;
    builder_tseitin builder(formula.clause_inserter(), nnf);

    const auto variable_count =
        reader_SMTLIB_tseitin(template_t<literal_pair>{}, renaming, builder)
            .read(input_parsed);

    auto begin_input = 1z;
    auto begin_introduced = greater_power_10(renaming.size()) + 1;

    output << "c The introduced variables begin at " << begin_introduced
           << ".\n";

    std::map<decltype(renaming)::mapped_type, decltype(renaming)::mapped_type>
        renaming_new;

    for (const auto& [_, name_input] : renaming)
    {
        renaming_new.try_emplace(name_input, begin_input++);
    }

    for (auto name_introduced = 0z; name_introduced != variable_count;
         ++name_introduced)
    {
        if (!renaming_new.contains(name_introduced))
        {
            renaming_new.try_emplace(name_introduced, begin_introduced++);
        }
    }

    for (const auto& [name, alias] : renaming)
    {
        output << "c " << renaming_new.at(alias) << "->" << name << "\n";
    }

    output << "p cnf " << variable_count << " " << formula.clauses_count()
           << "\n";

    formula.write_DIMACS(output,
                         [&renaming_new](const auto& x)
                         {
                             return literal_pair(renaming_new.at(x.variable),
                                                 x.is_positive());
                         });

    return 0;
}
