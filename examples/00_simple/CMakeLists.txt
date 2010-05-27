set(EX_NAME "00_example")

file(GLOB SRCS "*.cpp" "*.hpp")
add_executable(${EX_NAME} ${SRCS})
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set_target_properties( ${EX_NAME} PROPERTIES LINK_FLAGS "-Wl,-E")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
target_link_libraries(${EX_NAME} SLB)
