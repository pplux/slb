project( SLB )

set(EXECUTABLE_OUTPUT_PATH ${SLB_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${SLB_BINARY_DIR}/lib)
link_directories( ${LIBRARY_OUTPUT_PATH} )

include_directories( "${SLB_SOURCE_DIR}/include" )
include_directories( "${SLB_SOURCE_DIR}/include/SLB/lua" )

set(CMAKE_C_FLAGS_DEBUG "-Wall -g")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g")
#add_definitions(-DSLB_DEBUG_LEVEL=100)

# Header files...
file(GLOB HEADERS_SLB "${SLB_SOURCE_DIR}/include/SLB/*.hpp")
file(GLOB HEADERS_SLB_LUA "${SLB_SOURCE_DIR}/include/SLB/lua/*.h")
set(HEADERS ${HEADERS_SLB} ${HEADERS_SLB_LUA})
source_group("Header Files\\SLB" FILES ${HEADERS_SLB})
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

add_subdirectory(tests)
