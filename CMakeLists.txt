cmake_minimum_required(VERSION 2.4)
project( SLB )
set(EXECUTABLE_OUTPUT_PATH ${SLB_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${SLB_BINARY_DIR}/lib)

OPTION(SLB_BUILD_TESTS "Enable building Tests" OFF)
OPTION(SLB_BUILD_EXAMPLES "Enable examples" ON)
OPTION(SLB_USE_EMBEDDED_LUA "Use Lua embedded" ON)
OPTION(SLB_USE_VALGRIND "Use valgrind to check memory leaks" OFF)
SET(SLB_DEBUG_LEVEL 0)

link_directories( ${LIBRARY_OUTPUT_PATH} )

include_directories( "${SLB_SOURCE_DIR}/include" )

add_definitions(-DSLB_DEBUG_LEVEL=${SLB_DEBUG_LEVEL})
#TODO: make this an option
add_definitions(-DSLB_DEBUG_OUTPUT=stdout)

# SLB core files (headers and sources)
file(GLOB HEADERS_SLB "${SLB_SOURCE_DIR}/include/SLB/*.hpp")
file(GLOB SOURCES_SLB "${SLB_SOURCE_DIR}/src/*.cpp")

if(SLB_USE_EMBEDDED_LUA)
	include_directories( "${SLB_SOURCE_DIR}/include/SLB/lua" )
	file(GLOB HEADERS_SLB_LUA "${SLB_SOURCE_DIR}/include/SLB/lua/*.h")
	file(GLOB SOURCES_SLB_LUA "${SLB_SOURCE_DIR}/src/lua.c")
else(SLB_USE_EMBEDDED_LUA)
	add_definitions(-DSLB_EXTERNAL_LUA)
endif(SLB_USE_EMBEDDED_LUA)

set(HEADERS ${HEADERS_SLB} ${HEADERS_SLB_LUA})
set(SOURCES ${SOURCES_SLB} ${SOURCES_SLB_LUA})

add_library(SLB
	SHARED
	${HEADERS}
	${SOURCES}
)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-DSLB_LIBRARY)
	if (SLB_USE_EMBEDDED_LUA)
		add_definitions(-DLUA_BUILD_AS_DLL)
	else (SLB_USE_EMBEDDED_LUA)
		#TODO add here lua as external dependency
		message(FATAL "Missing configuration for lua as external dll in win32")
	endif(SLB_USE_EMBEDDED_LUA)
else(WIN32)
	set(CMAKE_C_FLAGS_DEBUG "-Wall -g -pg")
	set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -pg")

	if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		set_target_properties( SLB PROPERTIES LINK_FLAGS "-Wl,-E")
		target_link_libraries( SLB pthread)
	endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

	if (NOT SLB_USE_EMBEDDED_LUA)
		target_link_libraries( SLB lua dl)
	endif(NOT SLB_USE_EMBEDDED_LUA)

	if(SLB_USE_VALGRIND)
		add_definitions(-DUSE_VALGRIND)
	endif(SLB_USE_VALGRIND)
endif(WIN32)

if(SLB_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif(SLB_BUILD_TESTS)

if(SLB_BUILD_EXAMPLES)
	file(GLOB ALL_EXAMPLES "${SLB_SOURCE_DIR}/examples/*")
	foreach(example ${ALL_EXAMPLES})
		add_subdirectory(${example})
	endforeach(example)
endif(SLB_BUILD_EXAMPLES)
