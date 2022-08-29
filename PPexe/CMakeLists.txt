cmake_minimum_required(VERSION 3.20)

add_library("PPexe" SHARED
    "main.cpp"
)

set_target_properties("PPexe" PROPERTIES COMPILE_OPTIONS "-std=c++2b")
target_include_directories("PPexe" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../")

# Install

install(TARGETS "PPexe" RUNTIME)

# Utility
function(PPexe_add_executable name_)
    add_library("${name_}" SHARED)

    set(name_exe "${name_}-exe")

    add_executable("${name_exe}" "/dev/null")
    add_dependencies("${name_exe}" "PPexe" "${name_}")
    target_link_libraries("${name_exe}" PRIVATE "PPexe" "${name_}")
    set_target_properties("${name_exe}" PROPERTIES OUTPUT_NAME "${name_}" COMPILE_OPTIONS "-std=c++2b")
endfunction()
