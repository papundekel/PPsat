cmake_minimum_required(VERSION 3.20)

PPexe_add_executable("PPsat-crypt-generate")

target_sources("PPsat-crypt-generate" PUBLIC
    "main.cpp"
    "visitor_constraints.cpp"
    "visitor_variables.cpp"
)

set_target_properties("PPsat-crypt-generate" PROPERTIES COMPILE_OPTIONS "-std=c++2b")
target_include_directories("PPsat-crypt-generate" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../")
target_link_libraries("PPsat-crypt-generate" INTERFACE "PPsat-base")

# ANTLR

antlr4_generate(
    "PPsat-crypt-lexer"
    "${CMAKE_CURRENT_SOURCE_DIR}/../grammar/lexer_CRYPT.g4"
    LEXER
    FALSE
    FALSE
    "PPsat_crypt"
)

antlr4_generate(
    "PPsat-crypt-parser"
    "${CMAKE_CURRENT_SOURCE_DIR}/../grammar/parser_CRYPT.g4"
    PARSER
    FALSE
    TRUE
    "PPsat_crypt"
    "${ANTLR4_TOKEN_FILES_PPsat-crypt-lexer}"
    "${ANTLR4_TOKEN_DIRECTORY_PPsat-crypt-lexer}"
)

target_sources("PPsat-crypt-generate" PUBLIC
    "${ANTLR4_SRC_FILES_PPsat-crypt-lexer}"
    "${ANTLR4_SRC_FILES_PPsat-crypt-parser}"
)

target_include_directories("PPsat-crypt-generate" PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/../"
    "${ANTLR4_INCLUDE_DIR}"
    "${ANTLR4_INCLUDE_DIR_PPsat-crypt-lexer}/../"
)

add_dependencies("PPsat-crypt-generate" antlr4_shared)
target_link_libraries("PPsat-crypt-generate" INTERFACE antlr4_shared)

# Install

install(TARGETS "PPsat-crypt-generate" RUNTIME)
install(TARGETS "PPsat-crypt-generate-exe" RUNTIME)
install(PROGRAMS "PPsat-crypt.sh" TYPE BIN)
