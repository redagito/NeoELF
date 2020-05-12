set (TEST_NAME ${PROJECT_NAME}Test)

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hxx
)

add_executable (${TEST_NAME} ${SOURCE_FILES})
add_test (${TEST_NAME} ${TEST_NAME})

set_target_properties (${PROJECT_NAME} PROPERTIES 
	CXX_STANDARD 17
)

conan_cmake_run(REQUIRES 
	catch2/2.12.1

	BASIC_SETUP CMAKE_TARGETS
	BUILD missing
)

target_link_libraries(${TEST_NAME}
	PRIVATE ${PROJECT_NAME}Lib
	PRIVTAE CONAN_PKG::catch2
)

target_include_directories(${TEST_NAME} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
)