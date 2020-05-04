cmake_minimum_required (VERSION 3.16.0)
project (NeoELF)

# Download automatically
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/master/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)

# Engine sources
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

add_executable (${PROJECT_NAME} ${SOURCE_FILES})

set_target_properties (${PROJECT_NAME} PROPERTIES 
	CXX_STANDARD 17
)

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(REQUIRES 
	glm/0.9.9.5@g-truc/stable
	glfw/3.3@bincrafters/stable 
	glad/0.1.29@bincrafters/stable
	openal/1.19.0@bincrafters/stable
	vorbis/1.3.6@bincrafters/stable
	lua/5.3.4@zimmerk/stable
	
	jsonformoderncpp/3.7.0@vthiery/stable
	lodepng/1.0@int010h/stable
	
	OPTIONS glad:profile=core
	OPTIONS glad:api_type=gl
	OPTIONS glad:api_version=4.6
	OPTIONS glad:spec=gl
	OPTIONS glad:no_loader=False
	
	BASIC_SETUP CMAKE_TARGETS
	BUILD missing
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE CONAN_PKG::glm
	
	PRIVATE ${OPENGL_gl_LIBRARY}
	PRIVATE CONAN_PKG::glfw
	PRIVATE CONAN_PKG::glad
	
	PRIVATE CONAN_PKG::jsonformoderncpp
	PRIVATE CONAN_PKG::lodepng
	PRIVATE CONAN_PKG::openal
	PRIVATE CONAN_PKG::vorbis
	PRIVATE CONAN_PKG::lua
)

target_include_directories(${PROJECT_NAME} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE ${OPENGL_INCLUDE_DIR}
)

target_compile_definitions(${PROJECT_NAME} 
	PRIVATE -DGLM_ENABLE_EXPERIMENTAL
)