cmake_minimum_required(VERSION 3.20)

include_directories(${ANTLR4_INCLUDE_DIRS})

antlr4_generate(
    "PPsat-lexer"
    "${CMAKE_CURRENT_SOURCE_DIR}/../NNF_lexer.g4"
    LEXER
    FALSE
    FALSE
    "PPsat"
)

antlr4_generate(
    "PPsat-parser"
    "${CMAKE_CURRENT_SOURCE_DIR}/../NNF_parser.g4"
    PARSER
    FALSE
    TRUE
    "PPsat"
    "${ANTLR4_TOKEN_FILES_PPsat-lexer}"
    "${ANTLR4_TOKEN_DIRECTORY_PPsat-lexer}"
)

include_directories(
    "${ANTLR4_INCLUDE_DIR}"
    "${ANTLR4_INCLUDE_DIR_PPsat-lexer}"
    "${ANTLR4_INCLUDE_DIR_PPsat-parser}"
)

add_executable("PPsat"
    "main.cpp"
    "tseitin_translate.cpp"
    "${ANTLR4_SRC_FILES_PPsat-lexer}"
    "${ANTLR4_SRC_FILES_PPsat-parser}"
)

add_dependencies("PPsat" antlr4_shared)

target_link_libraries("PPsat" PRIVATE antlr4_shared)

target_include_directories("PPsat" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../")
set_property(TARGET "PPsat" PROPERTY CXX_STANDARD 20)
