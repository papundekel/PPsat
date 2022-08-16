cmake_minimum_required(VERSION 3.20)

add_library("PPsat-base" SHARED
    "antlrer.cpp"
    "builder.cpp"
    "clause.cpp"
    "cli/argument/file.cpp"
    "cli/argument/number.cpp"
    "cli/error_handler_simple.cpp"
    "cli/parameter.cpp"
    "cli/parser.cpp"
    "cli/option/bool.cpp"
    "cli/option/enum.cpp"
    "cli/option/named.cpp"
    "error_listener.cpp"
    "formula.cpp"
    "literal.cpp"
    "logger.cpp"
    "logger_ostream.cpp"
    "logger_subroutine.cpp"
    "tseitin_builder_nnf.cpp"
    "variable.cpp"
    "variable_representation.cpp"
    "variable_representation_always.cpp"
    "variable_representation_maybe.cpp"
)

set_target_properties("PPsat-base" PROPERTIES COMPILE_OPTIONS "-std=c++2b")
target_include_directories("PPsat-base" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../")

# ANTLR

target_include_directories("PPsat-base" PUBLIC "${ANTLR4_INCLUDE_DIR}")

add_dependencies("PPsat-base" antlr4_shared)
target_link_libraries("PPsat-base" PRIVATE antlr4_shared)

# Install

install(TARGETS "PPsat-base" LIBRARY)
