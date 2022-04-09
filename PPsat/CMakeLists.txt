cmake_minimum_required(VERSION 3.20)

add_executable("PPsat" "main.cpp" "translate.cpp")
target_include_directories("PPsat" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../")
