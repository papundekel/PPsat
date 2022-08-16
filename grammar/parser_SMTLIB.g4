/**
<formula> ::= `(' `and' <formula>  <formula> `)'
            | `(' `or'  <formula>  <formula> `)'
            | `(' `not' <formula> `)' 
            | <variable>
*/

parser grammar parser_SMTLIB;

@header
{
    #include <PPsat-base/antlr_parser.hpp>
    namespace PPsat { using antlr_parser = PPsat_base::antlr_parser; }
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
    tokenVocab=lexer_SMTLIB;
    superClass=antlr_parser;
}

input
    : formula EOF
    ;

formula
    : PARENTHESISL AND formula formula PARENTHESISR # Conjunction
    | PARENTHESISL OR  formula formula PARENTHESISR # Disjunction
    | PARENTHESISL NOT formula         PARENTHESISR # Negation
    | VAR                                           # Variable
    ;
