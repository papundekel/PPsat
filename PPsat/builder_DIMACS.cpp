#include <PPsat/builder_DIMACS.hpp>

#include <PPsat/visitor_DIMACS.hpp>

#include <PPsat-lexer_DIMACS/lexer_DIMACS.h>
#include <PPsat-parser_DIMACS/parser_DIMACS.h>

#include <memory>

bool PPsat::builder_DIMACS::read(const PPsat_base::logger& logger_outer,
                                 std::istream& input,
                                 PPsat_base::formula& formula) const
{
    return read_impl(logger_outer,
                     input,
                     PPsat_base::factory_lexer::impl<lexer_DIMACS>(),
                     PPsat_base::factory_parser::impl<parser_DIMACS>(),
                     visitor_DIMACS(formula));
}
