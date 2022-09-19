#include <PPsat-base/builder.hpp>

#include <PPsat-base/antlrer.hpp>
#include <PPsat-base/logger.hpp>
#include <PPsat-base/logger_subroutine.hpp>

bool PPsat_base::builder::read_impl(const logger& logger_outer,
                                    antlr4::CharStream& input,
                                    const factory_lexer& factory_lexer,
                                    const factory_parser& factory_parser,
                                    antlr4::tree::ParseTreeVisitor& visitor,
                                    bool lexer_error_is_fail)
{
    const auto logger_inner = logger_subroutine(logger_outer, "builder");

    antlrer antlrer(logger_inner,
                    input,
                    factory_lexer,
                    factory_parser,
                    lexer_error_is_fail);

    auto* const parse_tree = antlrer.parse();

    if (!parse_tree)
    {
        logger_inner << "Skipping the visitor.\n";
        return false;
    }

    return visitor.visit(parse_tree).has_value();
}
