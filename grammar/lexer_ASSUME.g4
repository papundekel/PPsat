lexer grammar lexer_ASSUME;

WHITESPACE: [ \t\r\n] -> channel(HIDDEN);

PREFIX: 'a';
END: '0';
NEGATED: '-';
VAR: [a-zA-Z0-9_]+;
