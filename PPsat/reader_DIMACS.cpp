#include "PPsat/renaming_map.hpp"
#include <PPsat/reader_DIMACS.hpp>

#include <PPsat/visitor_DIMACS.hpp>

#include <PPsat-lexer_DIMACS/lexer_DIMACS.h>
#include <PPsat-parser_DIMACS/parser_DIMACS.h>
#include <memory>

PPsat::reader::result PPsat::reader_DIMACS::read(std::istream& input,
                                                 formula& formula,
                                                 renaming& renaming) const
{
    return read_impl(input,
                     factory_lexer::impl<lexer_DIMACS>{},
                     factory_parser::impl<parser_DIMACS>{},
                     visitor_DIMACS(formula, renaming));
}

// std::unique_ptr<PPsat::renaming> PPsat::reader_DIMACS::create_renaming()
// const
// {
//     return std::make_unique<renaming_map<std::size_t>>();
// }
