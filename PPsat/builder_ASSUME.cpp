#include "PPsat-base/factory_lexer.hpp"
#include "PPsat/assumptions.hpp"
#include "PPsat/visitor_ASSUME.hpp"
#include <PPsat/builder_ASSUME.hpp>

#include <PPsat-lexer_ASSUME/lexer_ASSUME.h>
#include <PPsat-parser_ASSUME/parser_ASSUME.h>

PPsat::builder_ASSUME::builder_ASSUME(
    assumptions& assumption,
    const renaming& renaming_from_input) noexcept
    : assumption(assumption)
    , renaming_from_input(renaming_from_input)
{}

bool PPsat::builder_ASSUME::read(const PPsat_base::logger& logger_outer,
                                 antlr4::CharStream& input,
                                 bool lexer_error_is_fail) const
{
    visitor_ASSUME visitor(logger_outer, assumption, renaming_from_input);

    return read_impl(logger_outer,
                     input,
                     PPsat_base::factory_lexer::impl<lexer_ASSUME>(),
                     PPsat_base::factory_parser::impl<parser_ASSUME>(),
                     visitor,
                     lexer_error_is_fail);
}
