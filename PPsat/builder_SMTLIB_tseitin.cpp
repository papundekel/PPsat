#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/visitor_SMTLIB_tseitin.hpp>

#include <PPsat-lexer_SMTLIB/lexer_SMTLIB.h>
#include <PPsat-parser_SMTLIB/parser_SMTLIB.h>

#include <PPsat-base/conditional.hpp>
#include <PPsat-base/tseitin_builder_nnf.hpp>

#include <string_view>

bool PPsat::builder_SMTLIB_tseitin::read(const PPsat_base::logger& logger_outer,
                                         std::istream& input,
                                         PPsat_base::formula& formula) const
{
    return read_impl(
        logger_outer,
        input,
        PPsat_base::factory_lexer::impl<lexer_SMTLIB>(),
        PPsat_base::factory_parser::impl<parser_SMTLIB>(),
        visitor_SMTLIB_tseitin(conditional<PPsat_base::tseitin_builder>(
            nnf,
            PPsat_base::tseitin_builder_nnf<true>(formula),
            PPsat_base::tseitin_builder_nnf<false>(formula))));
}
