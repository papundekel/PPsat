cmake_minimum_required(VERSION 3.20)

add_custom_command(OUTPUT "call-graph.svg"
    COMMAND "perf" "script" "-i" "${CMAKE_CURRENT_LIST_DIR}/results-profiling.data" "|" "stackcollapse-perf.pl" "|" "flamegraph.pl" ">" "call-graph.svg"
    MAIN_DEPENDENCY "results-profiling.data"
    VERBATIM
)

add_custom_command(OUTPUT "report.nb.html"
    COMMAND "Rscript" "-e" "rmarkdown::render('${CMAKE_CURRENT_LIST_DIR}/report.Rmd', output_dir = '${CMAKE_CURRENT_BINARY_DIR}')"
    MAIN_DEPENDENCY "report.Rmd"
    DEPENDS "call-graph.svg"
    VERBATIM
)

add_custom_command(OUTPUT "report-performance.nb.html"
    COMMAND "Rscript" "-e" "rmarkdown::render('${CMAKE_CURRENT_LIST_DIR}/report-performance.Rmd', output_dir = '${CMAKE_CURRENT_BINARY_DIR}')"
    MAIN_DEPENDENCY "report-performance.Rmd"
    VERBATIM
)

add_custom_target(report ALL
    DEPENDS "report.nb.html" "report-performance.nb.html")

# Install

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/report.nb.html"
    "${CMAKE_CURRENT_BINARY_DIR}/report-performance.nb.html"
        TYPE DATA)

install(PROGRAMS
    "perf-perf.sh"
    "perf-warmup.sh"
        TYPE BIN)
