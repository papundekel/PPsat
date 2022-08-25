#pragma once
#include <PPsat-base/error_listener.hpp>
#include <PPsat-base/factory_lexer.hpp>
#include <PPsat-base/factory_parser.hpp>

#include <antlr4-runtime.h>

#include <memory>

namespace PPsat_base
{
class antlrer
{
    error_listener listener;
    const unique_ref<antlr4::Lexer> lexer;
    antlr4::CommonTokenStream token_stream;
    const unique_ref<PPsat_base::antlr_parser> parser;
    antlr4::ParserRuleContext* parsed_tree;

public:
    antlrer(const logger& logger_outer,
            antlr4::CharStream& input,
            const factory_lexer& factory_lexer,
            const factory_parser& factory_parser,
            bool lexer_error_is_fail);

    antlr4::ParserRuleContext* parse() const noexcept;
};
}
