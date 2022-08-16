cmake_minimum_required(VERSION 3.20)

add_executable("PPsat-crypt"
    "main.cpp"
    "clause.cpp"
    "variable.cpp"
    "visitor_constraints.cpp"
    "visitor_variable_def.cpp"
)

set_target_properties("PPsat-crypt" PROPERTIES COMPILE_OPTIONS "-std=c++2b")
target_include_directories("PPsat-queens-generate" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../")
target_link_libraries("PPsat-crypt" PUBLIC "PPsat-base")

# ANTLR

antlr4_generate(
    "PPsat-crypt-lexer_CRYPT"
    "${CMAKE_CURRENT_SOURCE_DIR}/../grammar/lexer_CRYPT.g4"
    LEXER
    FALSE
    FALSE
    "PPsat_crypt"
)

antlr4_generate(
    "PPsat-crypt-parser_CRYPT"
    "${CMAKE_CURRENT_SOURCE_DIR}/../grammar/parser_CRYPT.g4"
    PARSER
    FALSE
    TRUE
    "PPsat_crypt"
    "${ANTLR4_TOKEN_FILES_PPsat-crypt-lexer_CRYPT}"
    "${ANTLR4_TOKEN_DIRECTORY_PPsat-crypt-lexer_CRYPT}"
)

target_sources("PPsat-crypt" PUBLIC
    "${ANTLR4_SRC_FILES_PPsat-crypt-lexer_CRYPT}"
    "${ANTLR4_SRC_FILES_PPsat-crypt-parser_CRYPT}"
)

target_include_directories("PPsat-crypt" PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/../"
    "${ANTLR4_INCLUDE_DIR}"
    "${ANTLR4_INCLUDE_DIR_PPsat-crypt-lexer_CRYPT}/../"
)

add_dependencies("PPsat-crypt" antlr4_shared)
target_link_libraries("PPsat-crypt" PRIVATE antlr4_shared)

# Install

install(TARGETS "PPsat-crypt" RUNTIME)
