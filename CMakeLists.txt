cmake_minimum_required(VERSION 3.20)

project("PPsat" VERSION "1.0.0")

list(APPEND CMAKE_CONFIGURATION_TYPES "Profiling")
set(CMAKE_CXX_FLAGS_PROFILING "${CMAKE_CXX_FLAGS_PROFILING} -O0 -ggdb3 -fno-omit-frame-pointer -DNDEBUG")

include(CTest)
include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(DEFINED ENV{GEN_REPORT})
	set(GEN_REPORT $ENV{GEN_REPORT})
endif()

if(DEFINED ENV{GEN_SOLVER})
	set(GEN_SOLVER $ENV{GEN_SOLVER})
endif()

if(NOT DEFINED GEN_REPORT)
	set(GEN_REPORT OFF)
endif()

if(NOT DEFINED GEN_SOLVER)
	set(GEN_SOLVER OFF)
endif()

if(GEN_SOLVER)
	set(ANTLR4_TAG "master")
	include(ExternalAntlr4Cpp)

	find_package(ANTLR REQUIRED)
	set(ANTLR4_JAR_LOCATION "${ANTLR_EXECUTABLE}")

	find_package(antlr4-generator REQUIRED)
	find_package(antlr4-runtime REQUIRED)

	find_package(Boost 1.74 REQUIRED)

	if(APPLE)
		set(CMAKE_INSTALL_RPATH "@executable_path/../${CMAKE_INSTALL_LIBDIR}")
	elseif(UNIX)
		set(CMAKE_INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
	endif()

	include_directories("${PROJECT_SOURCE_DIR}")

	add_subdirectory("PPexe")
	add_subdirectory("PPsat-base")
	add_subdirectory("PPsat")
	add_subdirectory("PPsat-queens")
	add_subdirectory("PPsat-crypt")
endif()

if(GEN_REPORT)
	add_subdirectory("report")
endif()
