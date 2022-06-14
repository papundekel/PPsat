cmake_minimum_required(VERSION 3.20)

project("PPsat" VERSION "1.0.0")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(ExternalAntlr4Cpp)

set(ANTLR4_JAR_LOCATION "/usr/share/java/antlr-complete.jar")

find_package(ANTLR REQUIRED)
find_package(antlr4-generator REQUIRED)
find_package(antlr4-runtime REQUIRED)

add_subdirectory("PPsat")
add_subdirectory("examples")
