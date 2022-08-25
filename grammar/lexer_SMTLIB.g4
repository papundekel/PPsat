/**
<formula> ::= `(' `and' <formula>  <formula> `)'
            | `(' `or'  <formula>  <formula> `)'
            | `(' `not' <variable> `)' 
            | <variable>

Here <variable> is a sequence of alphanumeric characters starting with a letter.
Newline or a sequence of whitespace characters should be allowed wherever space is allowed.
*/

lexer grammar lexer_SMTLIB;

@header
{
    #include <PPsat-base/antlr_lexer.hpp>
    namespace PPsat { using antlr_lexer = PPsat_base::antlr_lexer; }
}

options
{
    superClass=antlr_lexer;
}

PARENTHESISL: '(';
PARENTHESISR: ')';
AND: 'and';
OR: 'or';
NOT: 'not';
VAR: [a-zA-Z] [a-zA-Z0-9_]*;
WHITESPACE: [ \t\r\n]+ -> channel(HIDDEN);
