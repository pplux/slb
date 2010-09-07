
file(GLOB SOURCES_TEST "${SLB_SOURCE_DIR}/tests/src/*.cpp" "${SLB_SOURCE_DIR}/tests/src/*.hpp")
add_executable(SLB_test ${SOURCES_TEST})
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set_target_properties( SLB_test PROPERTIES LINK_FLAGS "-Wl,-E")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
target_link_libraries(SLB_test SLB)

if(SLB_USE_VALGRIND)
	set (VALGRIND_OPTS  --leak-check=full --error-exitcode=124)
endif(SLB_USE_VALGRIND)

file(GLOB SCRIPTS_TEST "${SLB_SOURCE_DIR}/tests/scripts/test_*.lua")
foreach(script ${SCRIPTS_TEST})
	get_filename_component( TEST_NAME ${script} NAME)
	message( STATUS "Added test ${TEST_NAME}->${script}" )
	if(SLB_USE_VALGRIND)
		add_test("${TEST_NAME}" "valgrind" 
			${VALGRIND_OPTS} 
			"${EXECUTABLE_OUTPUT_PATH}/SLB_test" "${script}")
	else(SLB_USE_VALGRIND)
		add_test("${TEST_NAME}" 
			"${EXECUTABLE_OUTPUT_PATH}/SLB_test" "${script}")
	endif(SLB_USE_VALGRIND)
endforeach(script)

if(${WIN32})
	remove_definitions(-DLUA_BUILD_AS_DLL)
	remove_definitions(-DSLB_LIBRARY)
	# Copy dll to the same directory as the executable
	GET_TARGET_PROPERTY(SLB_DLL_OUTPUT_PATH SLB LOCATION)
	GET_TARGET_PROPERTY(SLB_TEST_OUTPUT_PATH SLB_test LOCATION)
	GET_FILENAME_COMPONENT(SLB_DLL_FINAL_PATH "${SLB_TEST_OUTPUT_PATH}" PATH)
	ADD_CUSTOM_COMMAND(
		TARGET SLB_test
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${SLB_DLL_OUTPUT_PATH}" "${SLB_DLL_FINAL_PATH}/SLB.dll"
		)
endif(${WIN32})
