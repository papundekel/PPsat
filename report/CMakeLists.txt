cmake_minimum_required(VERSION 3.20)

add_custom_command(OUTPUT "call-graph.svg"
    COMMAND "perf" "script" "-i" "${CMAKE_CURRENT_LIST_DIR}/results-profiling.data" "|" "stackcollapse-perf.pl" "|" "flamegraph.pl" ">" "call-graph.svg"
    MAIN_DEPENDENCY "results-profiling.data"
    VERBATIM
)

add_custom_command(OUTPUT "report-performance.html"
    COMMAND "jupyter" "nbconvert" "--to" "html" "--no-input" "--output-dir" "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_LIST_DIR}/report-performance.ipynb"
    MAIN_DEPENDENCY "report-performance.ipynb"
    DEPENDS "call-graph.svg"
    VERBATIM
)

add_custom_target(report ALL
    DEPENDS "report-performance.html")

# Install

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/report-performance.html"
    "${CMAKE_CURRENT_BINARY_DIR}/call-graph.svg"
        TYPE DATA)

install(PROGRAMS
    "perf-perf.sh"
    "perf-warmup.sh"
    "perf-profiling.sh"
    "perf-instr.sh"
        TYPE BIN)
