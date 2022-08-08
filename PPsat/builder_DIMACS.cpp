#include "PPsat/renaming_map.hpp"
#include <PPsat/builder_DIMACS.hpp>

#include <PPsat/visitor_DIMACS.hpp>

#include <PPsat-lexer_DIMACS/lexer_DIMACS.h>
#include <PPsat-parser_DIMACS/parser_DIMACS.h>
#include <memory>

PPsat::builder::result PPsat::builder_DIMACS::read(const logger& logger_outer,
                                                   std::istream& input,
                                                   formula& formula,
                                                   renaming& renaming) const
{
    return read_impl(logger_outer,
                     input,
                     factory_lexer::impl<lexer_DIMACS>{},
                     factory_parser::impl<parser_DIMACS>{},
                     visitor_DIMACS(formula, renaming));
}
