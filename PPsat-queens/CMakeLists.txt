cmake_minimum_required(VERSION 3.20)

add_executable("PPsat-queens-generate"
    "main.cpp"
)

set_target_properties("PPsat-queens-generate" PROPERTIES COMPILE_OPTIONS "-std=c++2b")

install(TARGETS "PPsat-queens-generate" RUNTIME)
install(PROGRAMS "PPsat-queens-measure.sh" "PPsat-queens.sh" TYPE BIN)
