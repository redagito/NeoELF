file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hxx
)
# Source group to preserve folder structure in ide
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

add_executable (${PROJECT_NAME} ${SOURCE_FILES})

set_target_properties (${PROJECT_NAME} PROPERTIES 
	CXX_STANDARD 17
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE ${PROJECT_NAME}Lib
)

target_include_directories(${PROJECT_NAME} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
)

# Copy runtime data
file (COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})