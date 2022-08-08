#include "PPsat/logger.hpp"
#include "PPsat/logger_subroutine.hpp"
#include <PPsat/builder.hpp>

#include <PPsat/antlrer.hpp>

PPsat::builder::result::result() noexcept
    : success(false)
    , variable_count(0)
{}

PPsat::builder::result::result(std::size_t variable_count) noexcept
    : success(true)
    , variable_count(variable_count)
{}

PPsat::builder::result::operator bool() const noexcept
{
    return success;
}

std::size_t PPsat::builder::result::get_variable_count() const noexcept
{
    return variable_count;
}

PPsat::builder::result PPsat::builder::read_impl(
    const logger& logger_outer,
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
        return {};
    }

    return std::any_cast<std::size_t>(visitor.visit(parse_tree));
}
