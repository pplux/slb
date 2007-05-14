project( SLB )

set(CMAKE_C_FLAGS_DEBUG "-Wall -g")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g")

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

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
include_directories( "${SLB_SOURCE_DIR}/include" )
link_directories( ${LIBRARY_OUTPUT_PATH} )

add_library(SLB
	SHARED
	${HEADERS}
	${SOURCES}
)

file(GLOB SOURCES_TEST "${SLB_SOURCE_DIR}/tests/src/*")
add_executable(SLB_test ${SOURCES_TEST})
target_link_libraries(SLB_test SLB)

enable_testing()

file(GLOB SCRIPTS_TEST "${SLB_SOURCE_DIR}/tests/scripts/*.lua")
foreach(script ${SCRIPTS_TEST})
	get_filename_component( TEST_NAME ${script} NAME)
	add_test(${TEST_NAME} "${EXECUTABLE_OUTPUT_PATH}/SLB_test" script)
endforeach(script)

