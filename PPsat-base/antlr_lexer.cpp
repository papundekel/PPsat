#include <PPsat-base/antlr_lexer.hpp>

#include <cstdlib>

void PPsat_base::antlr_lexer::recover(
    const antlr4::LexerNoViableAltException& e)
{
    type = SKIP;
    hitEOF = true;
}
