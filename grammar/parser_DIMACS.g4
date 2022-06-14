parser grammar parser_DIMACS;

options { tokenVocab=lexer_DIMACS; }

input
    : input_core EOF
    ;

input_core
    : P CNF NUMBER NUMBER clause*
    ;

clause
    : signed_number* ZERO
    ;

signed_number
    : MINUS? NUMBER
    ;
