cmake_minimum_required(VERSION 3.20)

add_executable("PPsat"
    "main.cpp"
    "adjacency_list.cpp"
    "adjacency_none.cpp"
    "adjacency_set.cpp"
    "adjacency_set_unordered.cpp"
    "adjacency_vector.cpp"
    "assumptions_basic.cpp"
    "builder_ASSUME.cpp"
    "builder_DIMACS.cpp"
    "builder_SMTLIB_tseitin.cpp"
    "clause_basic.cpp"
    "clause_counting.cpp"
    "clause_simple.cpp"
    "clause_watched_literals.cpp"
    "cli/argument/file.cpp"
    "cli/options.cpp"
    "create_builder.cpp"
    "conflict_analysis_dpll.cpp"
    "conflict_analysis_uip.cpp"
    "help_print.cpp"
    "heuristic_decision_assume.cpp"
    "heuristic_decision_first.cpp"
    "heuristic_decision_priority.cpp"
    "restart_strategy_never.cpp"
    "renaming_basic.cpp"
    "renaming_int.cpp"
    "renaming_int_basic.cpp"
    "solver.cpp"
    "subprogram/convert.cpp"
    "subprogram/solve.cpp"
    "variable_adjacency.cpp"
    "variable_assignable.cpp"
    "variable_assignable_not.cpp"
    "variable_antecedent_none.cpp"
    "variable_antecedent_with.cpp"
    "variable_level_none.cpp"
    "variable_level_with.cpp"
    "variable_recency_none.cpp"
    "variable_recency_with.cpp"
    "visitor_ASSUME.cpp"
    "visitor_DIMACS.cpp"
    "visitor_SMTLIB_tseitin.cpp"
)

set_target_properties("PPsat" PROPERTIES COMPILE_OPTIONS "-std=c++2b")
target_include_directories("PPsat" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../")
target_link_libraries("PPsat" PUBLIC "PPsat-base")

# ANTLR

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

antlr4_generate(
    "PPsat-lexer_ASSUME"
    "${CMAKE_CURRENT_SOURCE_DIR}/../grammar/lexer_ASSUME.g4"
    LEXER
    FALSE
    FALSE
    "PPsat"
)

antlr4_generate(
    "PPsat-parser_ASSUME"
    "${CMAKE_CURRENT_SOURCE_DIR}/../grammar/parser_ASSUME.g4"
    PARSER
    FALSE
    TRUE
    "PPsat"
    "${ANTLR4_TOKEN_FILES_PPsat-lexer_ASSUME}"
    "${ANTLR4_TOKEN_DIRECTORY_PPsat-lexer_ASSUME}"
)

target_sources("PPsat" PUBLIC
    "${ANTLR4_SRC_FILES_PPsat-lexer_SMTLIB}"
    "${ANTLR4_SRC_FILES_PPsat-parser_SMTLIB}"
    "${ANTLR4_SRC_FILES_PPsat-lexer_DIMACS}"
    "${ANTLR4_SRC_FILES_PPsat-parser_DIMACS}"
    "${ANTLR4_SRC_FILES_PPsat-lexer_ASSUME}"
    "${ANTLR4_SRC_FILES_PPsat-parser_ASSUME}"
)

target_include_directories("PPsat" PUBLIC
    "${ANTLR4_INCLUDE_DIR}"
    "${ANTLR4_INCLUDE_DIR_PPsat-lexer_SMTLIB}/../"
)

add_dependencies("PPsat" antlr4_shared)
target_link_libraries("PPsat" PRIVATE antlr4_shared)

# Test

set(inputs_dir "${CMAKE_CURRENT_LIST_DIR}/inputs")
set(regex_unsat "^s *UNSATISFIABLE")
set(regex_sat   "^s *SATISFIABLE")

file(GLOB inputs "${inputs_dir}/*/*")

foreach(file_path ${inputs})
    cmake_path(GET file_path STEM file_name)

    set(test_name_prefix "PPsat-solve")
    
    set(test_name_basic "${test_name_prefix}-basic-${file_name}")
    set(test_name_counting "${test_name_prefix}-counting-${file_name}")
    set(test_name_watched_literals "${test_name_prefix}-watched_literals-${file_name}")
    set(test_name_cdcl "${test_name_prefix}-cdcl-${file_name}")

    add_test(NAME "${test_name_basic}" COMMAND "PPsat" "-subprogram" "solve" "-clause" "basic" "-adjacency" "vector" "${file_path}" CONFIGURATIONS Release)
    add_test(NAME "${test_name_counting}" COMMAND "PPsat" "-subprogram" "solve" "-clause" "counting" "-adjacency" "vector" "${file_path}" CONFIGURATIONS Release)
    add_test(NAME "${test_name_watched_literals}" COMMAND "PPsat" "-subprogram" "solve" "-clause" "watched_literals" "-adjacency" "list" "${file_path}" CONFIGURATIONS Release)
    add_test(NAME "${test_name_cdcl}" COMMAND "PPsat" "-subprogram" "solve" "-cdcl" "-clause" "watched_literals" "-adjacency" "list" "${file_path}" CONFIGURATIONS Release)

    cmake_path(GET file_path PARENT_PATH parent_path)
    cmake_path(GET parent_path FILENAME parent_name)

    if(parent_name STREQUAL "unsat")
        set(regex_fail "${regex_sat}")
        set(regex_pass "${regex_unsat}")
    else()
        set(regex_fail "${regex_unsat}")
        set(regex_pass "${regex_sat}")
    endif()

    set_tests_properties(
        "${test_name_basic}"
        "${test_name_counting}"
        "${test_name_watched_literals}"
        #"${test_name_cdcl}"
    PROPERTIES
        FAIL_REGULAR_EXPRESSION "${regex_fail}"
        PASS_REGULAR_EXPRESSION "${regex_pass}"
    )
endforeach()

# Install

install(TARGETS "PPsat" RUNTIME)
