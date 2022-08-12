#pragma once
#include <PPsat/factory_lexer.hpp>
#include <PPsat/factory_parser.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/logger.hpp>

#include <iosfwd>
#include <memory>

namespace PPsat
{
class builder
{
public:
    class result
    {
        bool success;
        std::size_t variable_count;

    public:
        result() noexcept;
        result(std::size_t variable_count) noexcept;

        explicit operator bool() const noexcept;

        std::size_t get_variable_count() const noexcept;
    };

protected:
    static result read_impl(const logger& logger_outer,
                            std::istream& input,
                            const factory_lexer& factory_lexer,
                            const factory_parser& factory_parser,
                            antlr4::tree::ParseTreeVisitor&& visitor);

public:
    virtual result read(const logger& logger_outer,
                        std::istream& input,
                        formula& formula) const = 0;

    virtual ~builder() = default;
};
}
