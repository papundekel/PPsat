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
    "clause.cpp"
    "clause_sets.cpp"
    "clause_simple.cpp"
    "cli/argument/file.cpp"
    "cli/option/format.cpp"
    "cli/option/parser.cpp"
    "create_builder.cpp"
    "error_listener.cpp"
    "error_handler.cpp"
    "formula.cpp"
    "heuristic_decision_first.cpp"
    "literal.cpp"
    "logger.cpp"
    "logger_ostream.cpp"
    "logger_subroutine.cpp"
    "options.cpp"
    "builder.cpp"
    "builder_DIMACS.cpp"
    "builder_SMTLIB_tseitin.cpp"
    "subprogram/cdcl.cpp"
    "subprogram/convert.cpp"
    "subprogram/dpll.cpp"
    "subprogram/help.cpp"
    "tseitin_builder_nnf.cpp"
    "variable.cpp"
    "variable_adjacency.cpp"
    "variable_name_variant.cpp"
    "variable_simple.cpp"
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

set_target_properties("PPsat" PROPERTIES COMPILE_OPTIONS "-std=c++2b")

# Test

set(inputs_dir "${PROJECT_SOURCE_DIR}/inputs")
set(regex_unsat "^s *UNSATISFIABLE")
set(regex_sat   "^s *SATISFIABLE")

file(GLOB inputs "${inputs_dir}/*/*")

foreach(file_path ${inputs})
    cmake_path(GET file_path FILENAME file_name)

    set(test_name "PPsat-${file_name}")

    add_test(NAME "${test_name}" COMMAND "PPsat" "-dpll" "${file_path}")

    cmake_path(GET file_path PARENT_PATH parent_path)
    cmake_path(GET parent_path FILENAME parent_name)

    if(parent_name STREQUAL "unsat")
        set(regex_fail "${regex_sat}")
        set(regex_pass "${regex_unsat}")
    else()
        set(regex_fail "${regex_unsat}")
        set(regex_pass "${regex_sat}")
    endif()

    set_tests_properties("${test_name}" PROPERTIES
        FAIL_REGULAR_EXPRESSION "${regex_fail}"
        PASS_REGULAR_EXPRESSION "${regex_pass}"
    )
endforeach()

# Install

install(TARGETS "PPsat" RUNTIME)
