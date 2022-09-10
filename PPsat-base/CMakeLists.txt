cmake_minimum_required(VERSION 3.20)

add_library("PPsat-base" SHARED
    "antlr_lexer.cpp"
    "antlrer.cpp"
    "builder.cpp"
    "clause.cpp"
    "cli/argument/file.cpp"
    "cli/argument/number.cpp"
    "cli/parser.cpp"
    "cli/option/bool.cpp"
    "cli/option/enum.cpp"
    "cli/option/named.cpp"
    "cli/option/int.cpp"
    "cli/option/double.cpp"
    "error_listener.cpp"
    "formula.cpp"
    "literal.cpp"
    "logger.cpp"
    "logger_ostream.cpp"
    "logger_subroutine.cpp"
    "tseitin_builder_nnf.cpp"
    "variable.cpp"
    "variable_assignment.cpp"
    "variable_representation.cpp"
    "variable_representation_always.cpp"
    "variable_representation_maybe.cpp"
    "preprocessor_basic.cpp"
    "preprocessor_id.cpp"
)

set_target_properties("PPsat-base" PROPERTIES COMPILE_OPTIONS "-std=c++2b")

# ANTLR

target_include_directories("PPsat-base" PRIVATE "${ANTLR4_INCLUDE_DIR}")
target_link_libraries("PPsat-base" PRIVATE antlr4_shared)

# Install

install(TARGETS "PPsat-base" LIBRARY)
