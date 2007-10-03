
file(GLOB SOURCES_TEST "${SLB_SOURCE_DIR}/tests/src/*.cpp")
add_executable(SLB_test ${SOURCES_TEST})
target_link_libraries(SLB_test SLB)

set (VALGRIND_OPTS  --leak-check=full --error-exitcode=124)
file(GLOB SCRIPTS_TEST "${SLB_SOURCE_DIR}/tests/scripts/*.lua")
foreach(script ${SCRIPTS_TEST})
	get_filename_component( TEST_NAME ${script} NAME)
	message( STATUS "Added test ${TEST_NAME}->${script}" )
	add_test("${TEST_NAME}" "valgrind" ${VALGRIND_OPTS} "${EXECUTABLE_OUTPUT_PATH}/SLB_test" "${script}")
endforeach(script)
