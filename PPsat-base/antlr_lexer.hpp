#pragma once
#include <antlr4-runtime.h>

namespace PPsat_base
{
class antlr_lexer : public antlr4::Lexer
{
public:
    using Lexer::Lexer;

    void recover(const antlr4::LexerNoViableAltException& e) override final;
};
}
