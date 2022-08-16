#include <PPsat-base/antlrer.hpp>
#include <PPsat-base/logger.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <PPsat-base/builder.hpp>

bool PPsat_base::builder::read_impl(const logger& logger_outer,
                                    std::istream& input,
                                    const factory_lexer& factory_lexer,
                                    const factory_parser& factory_parser,
                                    antlr4::tree::ParseTreeVisitor&& visitor)
{
    const auto logger_inner = logger_subroutine(logger_outer, "builder");

    antlrer antlrer(logger_inner, input, factory_lexer, factory_parser);

    auto* const parse_tree = antlrer.parse();

    if (!parse_tree)
    {
        logger_inner << "Skipping building the formula.\n";
        return false;
    }

    visitor.visit(parse_tree);

    return true;
}
