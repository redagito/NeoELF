find_package (OpenGL REQUIRED)

set (LIB_NAME ${PROJECT_NAME}Lib)

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/source/*.hxx
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/*.hxx
)

add_library (${LIB_NAME} ${SOURCE_FILES})

set_target_properties (${LIB_NAME} PROPERTIES 
	CXX_STANDARD 17
)

conan_cmake_run(REQUIRES 
	glm/0.9.9.8
	glfw/3.3.2@bincrafters/stable 
	glad/0.1.33
	
	openal/1.19.1
	vorbis/1.3.6
	
	lua/5.3.5
	sol2/2.20.6
	bullet3/2.89
	freetype/2.10.1
	assimp/5.0.1
	nlohmann_json/3.7.0
	soil2/1.20@bincrafters/stable 
	
	OPTIONS glad:gl_profile=compatibility
	OPTIONS glad:gl_version=4.6
	OPTIONS glad:spec=gl
	OPTIONS glad:no_loader=False
	
	BASIC_SETUP CMAKE_TARGETS
	BUILD missing
)

target_link_libraries(${LIB_NAME}
	PUBLIC CONAN_PKG::glm
	
	PRIVATE ${OPENGL_gl_LIBRARY}
	PRIVATE CONAN_PKG::glfw
	PRIVATE CONAN_PKG::glad
	# Should be private, refactor audio to fix
	PUBLIC CONAN_PKG::openal
	PRIVATE CONAN_PKG::vorbis
	
	PRIVATE CONAN_PKG::lua
	PRIVATE CONAN_PKG::sol2
	PRIVATE CONAN_PKG::bullet3
	PRIVATE CONAN_PKG::freetype
	PRIVATE CONAN_PKG::assimp
	PRIVATE CONAN_PKG::nlohmann_json
	PRIVATE CONAN_PKG::soil2
)

target_include_directories(${LIB_NAME} 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
	PRIVATE ${OPENGL_INCLUDE_DIR}
)