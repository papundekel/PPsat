cmake_minimum_required(VERSION 3.20)

PPexe_add_executable("PPsat-queens-generate")
target_sources("PPsat-queens-generate" PUBLIC
    "main.cpp"
    "generate.cpp"
)

set_target_properties("PPsat-queens-generate" PROPERTIES COMPILE_OPTIONS "-std=c++2b")
target_link_libraries("PPsat-queens-generate" INTERFACE "PPsat-base")

# Install

install(TARGETS
    "PPsat-queens-generate"
    "PPsat-queens-generate-exe"
        RUNTIME)

install(PROGRAMS
    "PPsat-queens.sh"
    "PPsat-queens-measure.sh"
        TYPE BIN)
