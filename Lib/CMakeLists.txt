project(NeoElfLib)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)
# Source group to preserve folder structure in ide
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

add_library (${PROJECT_NAME} ${SOURCE_FILES})

set_target_properties (${PROJECT_NAME} PROPERTIES 
	CXX_STANDARD_REQUIRED YES
	CXX_STANDARD 17
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC glm::glm
	PRIVATE OpenGL::GL
	PRIVATE glfw
	PRIVATE glad::glad
	# Should be private, refactor audio to fix
	PUBLIC OpenAL::OpenAL
	PUBLIC vorbis::vorbis
	
	PRIVATE lua::lua
	PRIVATE sol2::sol2
	PRIVATE Bullet::Bullet
	PRIVATE Freetype::Freetype
	PRIVATE assimp::assimp
	PRIVATE nlohmann_json::nlohmann_json
	PRIVATE stb::stb
)

target_include_directories(${PROJECT_NAME} 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
)