include_directories(../src)

list (APPEND SOURCES test-reader.c test-expect.c test-buffer.c test-file.c test-node.c test-write.c test-common.c test-system.c)
list (APPEND LIBRARIES mpack)

if(YOTTA_CFG_MBED)
	list (APPEND SOURCES testMBED.cpp)
	list (APPEND LIBRARIES mbed-drivers)
else(YOTTA_CFG_MBED)
	list (APPEND SOURCES test.c)
endif(YOTTA_CFG_MBED)

add_executable(mpacktest ${SOURCES})


target_link_libraries(mpacktest ${LIBRARIES})

if(YOTTA_CFG_MBED)
	#yotta_postprocess_target(EXECUTABLE mpacktest)
	add_test(mpack-test-mpacktest mpacktest)
	add_dependencies(all_tests mpacktest)
endif(YOTTA_CFG_MBED)