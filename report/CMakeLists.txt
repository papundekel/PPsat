cmake_minimum_required(VERSION 3.20)

add_custom_command(OUTPUT "report.nb.html"
    COMMAND "Rscript" "-e" "rmarkdown::render('${CMAKE_CURRENT_LIST_DIR}/report.Rmd', output_dir ='${CMAKE_CURRENT_BINARY_DIR}')"
    MAIN_DEPENDENCY "report.Rmd"
    VERBATIM
)

add_custom_target(report ALL
    DEPENDS "report.nb.html")

# Install

install()

install(PROGRAMS
    "perf.sh"
        TYPE BIN)
