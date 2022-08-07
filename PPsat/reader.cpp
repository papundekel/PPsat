#include <PPsat/reader.hpp>

#include <PPsat/antlrer.hpp>

PPsat::reader::result::result() noexcept
    : success(false)
    , variable_count(0)
{}

PPsat::reader::result::result(std::size_t variable_count) noexcept
    : success(true)
    , variable_count(variable_count)
{}

PPsat::reader::result::operator bool() const noexcept
{
    return success;
}

std::size_t PPsat::reader::result::get_variable_count() const noexcept
{
    return variable_count;
}

PPsat::reader::result PPsat::reader::read_impl(
    std::istream& input,
    const factory_lexer& factory_lexer,
    const factory_parser& factory_parser,
    antlr4::tree::ParseTreeVisitor&& visitor)
{
    antlrer antlrer(input, factory_lexer, factory_parser);

    auto* const parse_tree = antlrer.parse();

    if (!parse_tree)
    {
        return {};
    }

    return std::any_cast<std::size_t>(visitor.visit(parse_tree));
}
