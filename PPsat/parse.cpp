#include <PPsat/parse.hpp>

#include <PPsat/antlrer.hpp>
#include <PPsat/conditional.hpp>
#include <PPsat/tseitin_builder.hpp>

#include <PPsat-lexer_DIMACS/lexer_DIMACS.h>
#include <PPsat-lexer_SMTLIB/lexer_SMTLIB.h>
#include <PPsat-parser_DIMACS/parser_DIMACS.h>
#include <PPsat-parser_SMTLIB/parser_SMTLIB.h>

bool PPsat::parse(std::istream& input,
                  formula_format format,
                  bool nnf,
                  formula& formula,
                  renaming& renaming,
                  std::size_t& count_variable)
{
    antlrer antlrer(input,
                    conditional<const antlrer::factory_lexer_t&>(
                        format == formula_format::SMTLIB,
                        antlrer::factory_lexer_t::impl<lexer_SMTLIB>{},
                        antlrer::factory_lexer_t::impl<lexer_DIMACS>{}),
                    conditional<const antlrer::factory_parser_t&>(
                        format == formula_format::SMTLIB,
                        antlrer::factory_parser_t::impl<parser_SMTLIB>{},
                        antlrer::factory_parser_t::impl<parser_DIMACS>{}));

    auto* const parse_tree = antlrer.parse();

    if (!parse_tree)
    {
        return false;
    }

    count_variable =
        reader_SMTLIB_tseitin(renaming,
                              conditional<const tseitin_builder&>(
                                  nnf,
                                  tseitin_builder_nnf<true>(formula),
                                  tseitin_builder_nnf<false>(formula)),
                              parse_tree)
            .result();
}
