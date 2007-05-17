project( SLB )

# use the external output for library binaries...
if(NOT SLB_EXTERNAL)
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
	set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
	link_directories( ${LIBRARY_OUTPUT_PATH} )
endif(NOT SLB_EXTERNAL)

include_directories( "${SLB_SOURCE_DIR}/include" )

set(CMAKE_C_FLAGS_DEBUG "-Wall -g")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g")
#add_definitions(-DSLB_DEBUG_LEVEL=100)

# Header files...
file(GLOB HEADERS_SLB "${SLB_SOURCE_DIR}/include/*.hpp")
file(GLOB HEADERS_SLB_SLB "${SLB_SOURCE_DIR}/include/SLB/*.hpp")
file(GLOB HEADERS_SLB_LUA "${SLB_SOURCE_DIR}/include/SLB/lua/*.h")
set(HEADERS ${HEADERS_SLB} ${HEADERS_SLB_SLB} ${HEADERS_SLB_LUA})
source_group("Header Files\\SLB" FILES ${HEADERS_SLB} ${HEADERS_SLB_SLB})
source_group("Header Files\\SLB\\lua" FILES ${HEADERS_SLB_LUA})

# Source files...
file(GLOB SOURCES_SLB "${SLB_SOURCE_DIR}/src/*.cpp")
file(GLOB SOURCES_SLB_LUA "${SLB_SOURCE_DIR}/src/lua/*.c")
set(SOURCES ${SOURCES_SLB} ${SOURCES_SLB_LUA})
source_group("Source Files\\SLB" FILES ${SOURCES_SLB})
source_group("Source Files\\SLB\\lua" FILES ${SOURCES_SLB_LUA})

add_library(SLB
	SHARED
	${HEADERS}
	${SOURCES}
)
set_target_properties( SLB PROPERTIES LINK_FLAGS "-Wl,-E")

# Compiling SLB as external means not compiling tests and so on
if(NOT SLB_EXTERNAL)
	file(GLOB SOURCES_TEST "${SLB_SOURCE_DIR}/tests/src/*")
	add_executable(SLB_test ${SOURCES_TEST})
	target_link_libraries(SLB_test SLB)

	enable_testing()

	file(GLOB SCRIPTS_TEST "${SLB_SOURCE_DIR}/tests/scripts/*.lua")
	foreach(script ${SCRIPTS_TEST})
		get_filename_component( TEST_NAME ${script} NAME)
		add_test("${TEST_NAME}" "${EXECUTABLE_OUTPUT_PATH}/SLB_test" "${script}")
	endforeach(script)
endif(NOT SLB_EXTERNAL)
