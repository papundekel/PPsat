cmake_minimum_required(VERSION 3.20)

PPexe_add_executable("PPsat")
target_sources("PPsat" PUBLIC
    "main.cpp"
    "assumptions_basic.cpp"
    "builder_ASSUME.cpp"
    "builder_DIMACS.cpp"
    "builder_SMTLIB_tseitin.cpp"
    "clause.cpp"
    "creator.cpp"
    "cli/argument/file.cpp"
    "cli/parameters.cpp"
    "create_builder.cpp"
    "conflict_analysis_dpll.cpp"
    "conflict_analysis_uip.cpp"
    "help_print.cpp"
    "decision_assume.cpp"
    "decision_deterministic.cpp"
    "decision_trivial.cpp"
    "decision_priority.cpp"
    "decision_random.cpp"
    "decision_JW_static.cpp"
    "decision_VSIDS.cpp"
    "literal.cpp"
    "preprocessor_basic.cpp"
    "preprocessor_id.cpp"
    "renaming_basic.cpp"
    "renaming_int.cpp"
    "renaming_int_basic.cpp"
    "subprogram/convert.cpp"
    "subprogram/invalid.cpp"
    "subprogram/solve.cpp"
    "tseitin_builder_nnf.cpp"
    "variable.cpp"
    "variable_assignment.cpp"
    "visitor_ASSUME.cpp"
    "visitor_DIMACS.cpp"
    "visitor_SMTLIB_tseitin.cpp"
)

target_compile_options("PPsat"
    PUBLIC "-std=c++2b"
    PUBLIC "-ftemplate-depth=1500"
    PUBLIC "-fconstexpr-depth=10000"
    PUBLIC "-fconstexpr-ops-limit=1000000000")
target_include_directories("PPsat" PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../")
target_link_libraries("PPsat" INTERFACE "PPsat-base")

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
target_link_libraries("PPsat" INTERFACE antlr4_shared)

# Test

set(inputs_dir "${CMAKE_CURRENT_LIST_DIR}/inputs")
set(regex_unsat "^s *UNSATISFIABLE")
set(regex_sat   "^s *SATISFIABLE")

file(GLOB inputs "${inputs_dir}/*/*")

set(option_adjacency_list )
set(option_clause_list )
set(option_decision_list )

foreach(option_virtual "true" "false")
foreach(option_cdcl "true" "false")
foreach(option_adjacency "vector" "list" "set" "set_unordered")
foreach(option_clause "basic" "counting" "watched_literals")
foreach(option_decision "deterministic" "trivial" "random" "JW_static" "VSIDS")
foreach(file_path ${inputs})
    cmake_path(GET file_path STEM file_name)

    set(test_name_prefix "PPsat-solve")
    
    cmake_path(GET file_path PARENT_PATH parent_path)
    cmake_path(GET parent_path FILENAME parent_name)

    if(parent_name STREQUAL "unsat")
        set(regex_fail "${regex_sat}")
        set(regex_pass "${regex_unsat}")
    else()
        set(regex_fail "${regex_unsat}")
        set(regex_pass "${regex_sat}")
    endif()

    set(test_name "PPsat-solve-${option_adjacency}-${option_clause}-${option_decision}-${file_name}-${option_cdcl}-${option_virtual}")

    add_test(
    NAME "${test_name}"
    COMMAND "PPsat"
        "-subprogram" "solve"
        "-iterations" "2"
        "-adjacency" "${option_adjacency}"
        "-clause" "${option_clause}"
        "-decision" "${option_decision}"
        "-cdcl" "${option_cdcl}"
        "-virtual" "${option_virtual}"
        "${file_path}"
    CONFIGURATIONS Release)

    set_tests_properties(
        "${test_name}"
    PROPERTIES
        FAIL_REGULAR_EXPRESSION "${regex_fail}"
        PASS_REGULAR_EXPRESSION "${regex_pass}"
    )
endforeach()
endforeach()
endforeach()
endforeach()
endforeach()
endforeach()

# Install

install(TARGETS "PPsat" RUNTIME)
install(TARGETS "PPsat-exe" RUNTIME)
install(PROGRAMS "PPsat-measure.sh" TYPE BIN)
