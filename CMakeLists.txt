cmake_minimum_required(VERSION 3.20)

project("PPsat" VERSION "1.0.0")

include(CTest)
include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(ExternalAntlr4Cpp)

set(ANTLR4_JAR_LOCATION "/usr/share/java/antlr-complete.jar")

find_package(ANTLR REQUIRED)
find_package(antlr4-generator REQUIRED)
find_package(antlr4-runtime REQUIRED)

list(APPEND CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

add_subdirectory("PPexe")
add_subdirectory("PPsat-base")
add_subdirectory("PPsat")
add_subdirectory("PPsat-queens")
add_subdirectory("PPsat-crypt")
