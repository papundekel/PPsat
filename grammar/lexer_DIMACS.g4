lexer grammar lexer_DIMACS;

@header
{
    #include <PPsat-base/antlr_lexer.hpp>
    namespace PPsat { using antlr_lexer = PPsat_base::antlr_lexer; }
}

options
{
    superClass=antlr_lexer;
}

WHITESPACE: [ \t\r\n] -> channel(HIDDEN);
COMMENT: 'c' (~[\r\n])* -> channel(HIDDEN);

MAGIC: 'p cnf';
NEGATED: '-';
CLAUSE_END: '0';
NUMBER: [1-9] [0-9]*;
