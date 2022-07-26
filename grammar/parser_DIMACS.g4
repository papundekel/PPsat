parser grammar parser_DIMACS;

options { tokenVocab=lexer_DIMACS; }

input
    : P CNF NUMBER NUMBER clause* EOF
    ;

clause
    : literal* ZERO
    ;

literal
    : MINUS? NUMBER
    ;
