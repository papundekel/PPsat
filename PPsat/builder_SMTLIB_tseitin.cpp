#include <PPsat/builder_SMTLIB_tseitin.hpp>

#include <PPsat/conditional.hpp>
#include <PPsat/renaming_map.hpp>
#include <PPsat/tseitin_builder_nnf.hpp>
#include <PPsat/visitor_SMTLIB_tseitin.hpp>

#include <PPsat-lexer_SMTLIB/lexer_SMTLIB.h>
#include <PPsat-parser_SMTLIB/parser_SMTLIB.h>

#include <string_view>

PPsat::builder::result PPsat::builder_SMTLIB_tseitin::read(
    const logger& logger_outer,
    std::istream& input,
    formula& formula,
    renaming& renaming) const
{
    return read_impl(
        logger_outer,
        input,
        factory_lexer::impl<lexer_SMTLIB>{},
        factory_parser::impl<parser_SMTLIB>{},
        visitor_SMTLIB_tseitin(
            conditional<tseitin_builder>(nnf,
                                         tseitin_builder_nnf<true>(formula),
                                         tseitin_builder_nnf<false>(formula)),
            renaming));
}
