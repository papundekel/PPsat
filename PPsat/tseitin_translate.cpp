#include <PPsat/error_listener_simple_detect.hpp>
#include <PPsat/reader_SMTLIB_tseitin.hpp>
#include <PPsat/tseitin_translate.hpp>

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
}

PPsat::error_code PPsat::tseitin_translate(std::istream& input,
                                           std::ostream& output, bool nnf)
{
    error_listener_simple_detect error_listener;

    antlr4::ANTLRInputStream input_antlr(input);

    lexer_SMTLIB lexer(&input_antlr);
    lexer.addErrorListener(&error_listener);

    antlr4::CommonTokenStream tokens(&lexer);

    parser_SMTLIB parser(&tokens);
    parser.addErrorListener(&error_listener);

    auto* input_parsed = parser.input();

    if (error_listener.error_encountered())
    {
        return error_code::syntax;
    }

    auto [formula, renaming, variable_count] =
        reader_SMTLIB_tseitin().read(input_parsed, nnf);

    std::size_t begin_input = 1;
    auto begin_introduced = greater_power_10(renaming.size()) + 1;

    output << "c The introduced variables begin at " << begin_introduced << ".\n";

    std::map<decltype(renaming)::mapped_type, decltype(renaming)::mapped_type> renaming_new;

    for (const auto& [_, name_input] : renaming)
    {
        renaming_new.try_emplace(name_input, begin_input++);
    }

    for (std::size_t name_introduced = 0; name_introduced != variable_count; ++name_introduced)
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

    return error_code::none;
}
