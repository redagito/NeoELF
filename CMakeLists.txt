cmake_minimum_required (VERSION 3.25.0)
project(NeoELF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Dependencies
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(glad REQUIRED)
find_package(assimp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(stb REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenAL REQUIRED)
find_package(vorbis REQUIRED)
find_package(lua REQUIRED)
find_package(sol2 REQUIRED)
find_package(Bullet REQUIRED)
find_package(fmtlog REQUIRED)
find_package(catch2 REQUIRED)

# NeoELF library
add_subdirectory(Lib)

# Unit tests
add_subdirectory (Test)

# Engine application
add_subdirectory (App)

# App as startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT NeoElfApp)