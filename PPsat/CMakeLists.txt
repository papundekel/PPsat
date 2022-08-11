cmake_minimum_required(VERSION 3.20)

include_directories(${ANTLR4_INCLUDE_DIRS})

antlr4_generate(
    "PPsat-lexer_SMTLIB"
    "${CMAKE_CURRENT_SOURCE_DIR}/../grammar/lexer_SMTLIB.g4"
    LEXER
    FALSE
    FALSE
    "PPsat"
)

antlr4_generate(
    "PPsat-parser_SMTLIB"
    "${CMAKE_CURRENT_SOURCE_DIR}/../grammar/parser_SMTLIB.g4"
    PARSER
    FALSE
    TRUE
    "PPsat"
    "${ANTLR4_TOKEN_FILES_PPsat-lexer_SMTLIB}"
    "${ANTLR4_TOKEN_DIRECTORY_PPsat-lexer_SMTLIB}"
)

antlr4_generate(
    "PPsat-lexer_DIMACS"
    "${CMAKE_CURRENT_SOURCE_DIR}/../grammar/lexer_DIMACS.g4"
    LEXER
    FALSE
    FALSE
    "PPsat"
)

antlr4_generate(
    "PPsat-parser_DIMACS"
    "${CMAKE_CURRENT_SOURCE_DIR}/../grammar/parser_DIMACS.g4"
    PARSER
    FALSE
    TRUE
    "PPsat"
    "${ANTLR4_TOKEN_FILES_PPsat-lexer_DIMACS}"
    "${ANTLR4_TOKEN_DIRECTORY_PPsat-lexer_DIMACS}"
)

add_executable("PPsat"
    "main.cpp"
    "antlrer.cpp"
    "cli/argument/file.cpp"
    "cli/option/format.cpp"
    "cli/option/parser.cpp"
    "create_builder.cpp"
    "error_listener.cpp"
    "error_handler.cpp"
    "formula.cpp"
    "formula_simple.cpp"
    "literal.cpp"
    "literal_negated.cpp"
    "literal_pair.cpp"
    "literal_signed.cpp"
    "logger.cpp"
    "logger_ostream.cpp"
    "logger_subroutine.cpp"
    "options.cpp"
    "builder.cpp"
    "builder_DIMACS.cpp"
    "builder_SMTLIB_tseitin.cpp"
    "renaming.cpp"
    "renaming_map.cpp"
    "subprogram/cdcl.cpp"
    "subprogram/convert.cpp"
    "subprogram/dpll.cpp"
    "subprogram/help.cpp"
    "visitor_DIMACS.cpp"
    "visitor_SMTLIB_tseitin.cpp"
    "${ANTLR4_SRC_FILES_PPsat-lexer_SMTLIB}"
    "${ANTLR4_SRC_FILES_PPsat-parser_SMTLIB}"
    "${ANTLR4_SRC_FILES_PPsat-lexer_DIMACS}"
    "${ANTLR4_SRC_FILES_PPsat-parser_DIMACS}"
)

add_dependencies("PPsat" antlr4_shared)

target_link_libraries("PPsat" PRIVATE antlr4_shared)

target_include_directories("PPsat" PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/../"
    "${ANTLR4_INCLUDE_DIR}"
    "${ANTLR4_INCLUDE_DIR_PPsat-lexer_SMTLIB}/../"
)

set_target_properties(PPsat PROPERTIES
    COMPILE_FLAGS "-std=c++2b"
    #COMPILE_FLAGS "-fconcepts-diagnostics-depth=6"
)
