cmake_minimum_required(VERSION 3.16)
project(FPSLightEngine)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

add_subdirectory(external/glfw)

include_directories(external/glad/include)

include_directories(external/glm)

include_directories(src)

file(GLOB_RECURSE SOURCES src/*.cpp)
list(APPEND SOURCES external/glad/src/glad.c)

add_executable(FPSLightEngine ${SOURCES})

target_link_libraries(FPSLightEngine glfw OpenGL::GL)
