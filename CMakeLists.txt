cmake_minimum_required(VERSION 3.20)

project("PPsat" VERSION "1.0.0")

include(CTest)
include(GNUInstallDirs)

list(APPEND CMAKE_PREFIX_PATH
	"${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib/cmake"
	"${ANTLR_DIR}"
)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (ANTLR4_TAG "master")
include(ExternalAntlr4Cpp)

if (NOT BOOTSTRAP)
	find_package(ANTLR REQUIRED)
	set(ANTLR4_JAR_LOCATION "${ANTLR_EXECUTABLE}")

	find_package(antlr4-generator REQUIRED)
	find_package(antlr4-runtime REQUIRED)

	if(APPLE)
		set(CMAKE_INSTALL_RPATH "@executable_path/../${CMAKE_INSTALL_LIBDIR}")
	elseif(UNIX)
		set(CMAKE_INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
	endif()

	add_subdirectory("PPexe")
	add_subdirectory("PPsat-base")
	add_subdirectory("PPsat")
	add_subdirectory("PPsat-queens")
	add_subdirectory("PPsat-crypt")
endif()
