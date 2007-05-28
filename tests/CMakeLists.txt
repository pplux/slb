
# Compiling SLB as external means not compiling tests and so on
	file(GLOB SOURCES_TEST "${SLB_SOURCE_DIR}/tests/src/*")
	add_executable(SLB_test ${SOURCES_TEST})
	target_link_libraries(SLB_test SLB)

	enable_testing()

	file(GLOB SCRIPTS_TEST "${SLB_SOURCE_DIR}/tests/scripts/*.lua")
	foreach(script ${SCRIPTS_TEST})
		get_filename_component( TEST_NAME ${script} NAME)
		add_test("${TEST_NAME}" "${EXECUTABLE_OUTPUT_PATH}/SLB_test" "${script}")
	endforeach(script)
