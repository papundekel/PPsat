#include <PPsat/builder_DIMACS.hpp>

#include <PPsat/visitor_DIMACS.hpp>

#include <PPsat-lexer_DIMACS/lexer_DIMACS.h>
#include <PPsat-parser_DIMACS/parser_DIMACS.h>

#include <memory>

PPsat::builder_DIMACS::builder_DIMACS(
    PPsat_base::formula& formula,
    renaming_int& renaming_from_input) noexcept
    : formula(formula)
    , renaming_from_input(renaming_from_input)
{}

bool PPsat::builder_DIMACS::read(const PPsat_base::logger& logger_outer,
                                 antlr4::CharStream& input,
                                 bool lexer_error_is_fail) const
{
    visitor_DIMACS visitor(logger_outer, formula, renaming_from_input);

    return read_impl(logger_outer,
                     input,
                     PPsat_base::factory_lexer::impl<lexer_DIMACS>(),
                     PPsat_base::factory_parser::impl<parser_DIMACS>(),
                     visitor,
                     lexer_error_is_fail);
}
