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
    "tseitin_translate.cpp"
    "error_listener_simple_detect.cpp"
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
set_property(TARGET "PPsat" PROPERTY CXX_STANDARD 20)
