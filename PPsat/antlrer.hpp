#pragma once
#include <PPsat/error_listener_simple_detect.hpp>
#include <PPsat/factory.hpp>
#include <PPsat/parser_abstract.hpp>

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
    std::unique_ptr<parser_abstract> parser;
    antlr4::ParserRuleContext* parsed_tree;

public:
    using factory_lexer_t = factory<antlr4::Lexer, antlr4::CharStream*>;
    using factory_parser_t = factory<parser_abstract, antlr4::TokenStream*>;

    antlrer(std::istream& input,
            const factory_lexer_t& factory_lexer,
            const factory_parser_t& factory_parser);

    antlr4::ParserRuleContext* parse() const noexcept;
};
}
