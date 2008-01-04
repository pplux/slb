project( SLB )

OPTION(SLB_BUILD_TESTS "Enable building Tests" OFF)
OPTION(SLB_USE_EMBEDDED_LUA "Use Lua embedded" ON)
SET(SLB_DEBUG_LEVEL 0)

set(EXECUTABLE_OUTPUT_PATH ${SLB_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${SLB_BINARY_DIR}/lib)
link_directories( ${LIBRARY_OUTPUT_PATH} )

include_directories( "${SLB_SOURCE_DIR}/include" )

set(CMAKE_C_FLAGS_DEBUG "-Wall -g")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g")
add_definitions(-DSLB_DEBUG_LEVEL=${SLB_DEBUG_LEVEL})

# SLB core files (headers and sources)
file(GLOB HEADERS_SLB "${SLB_SOURCE_DIR}/include/SLB/*.hpp")
source_group("Header Files\\SLB" FILES ${HEADERS_SLB})
file(GLOB SOURCES_SLB "${SLB_SOURCE_DIR}/src/*.cpp")
source_group("Source Files\\SLB" FILES ${SOURCES_SLB})

if(SLB_USE_EMBEDDED_LUA)
	include_directories( "${SLB_SOURCE_DIR}/include/SLB/lua" )
	file(GLOB HEADERS_SLB_LUA "${SLB_SOURCE_DIR}/include/SLB/lua/*.h")
	source_group("Header Files\\SLB\\lua" FILES ${HEADERS_SLB_LUA})
	file(GLOB SOURCES_SLB_LUA "${SLB_SOURCE_DIR}/src/lua.c")
	source_group("Source Files\\SLB\\lua" FILES ${SOURCES_SLB_LUA})
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

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set_target_properties( SLB PROPERTIES LINK_FLAGS "-Wl,-E")
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if (NOT SLB_USE_EMBEDDED_LUA)
	target_link_libraries( SLB lua dl)
endif(NOT SLB_USE_EMBEDDED_LUA)

if(SLB_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif(SLB_BUILD_TESTS)
