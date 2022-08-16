parser grammar parser_CRYPT;

@header
{
    #include <PPsat-base/antlr_parser.hpp>
    namespace PPsat_crypt { using antlr_parser = PPsat_base::antlr_parser; }
}

@members
{
    antlr4::ParserRuleContext* getParsedTree() override final
    {
        return input();
    }
}

options
{
    tokenVocab=lexer_CRYPT;
    superClass=antlr_parser;
}

input
    : formula EOF
    ;

formula
    : PARENL formula PARENR # Parentheses
    | NOT formula           # Negation
    | formula AND formula   # Conjunction
    | formula OR formula    # Disjunction
    | atom                  # AtomFormula
    ;

atom
    : WORD (operation WORD)* EQUAL WORD
    ;

operation
    : ADD | SUB
    ;
