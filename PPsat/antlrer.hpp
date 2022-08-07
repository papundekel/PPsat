#pragma once
#include <PPsat/error_listener_simple_detect.hpp>
#include <PPsat/factory_lexer.hpp>
#include <PPsat/factory_parser.hpp>

#include <antlr4-runtime.h>

#include <memory>

namespace PPsat
{
class antlrer
{
    error_listener_simple_detect error_listener;
    antlr4::ANTLRInputStream input_antlr;
    std::unique_ptr<antlr4::Lexer> lexer;
    antlr4::CommonTokenStream token_stream;
    std::unique_ptr<antlr_parser> parser;
    antlr4::ParserRuleContext* parsed_tree;

public:
    antlrer(std::istream& input,
            const factory_lexer& factory_lexer,
            const factory_parser& factory_parser);

    antlr4::ParserRuleContext* parse() const noexcept;
};
}
