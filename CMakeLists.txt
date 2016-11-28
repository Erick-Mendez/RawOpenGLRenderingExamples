cmake_minimum_required(VERSION 2.8)

project(RawOpenGLRenderingExamples)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

add_definitions(-Wall -Wpedantic -Werror)

include_directories(
    ${GL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIR}
)

link_directories(
    ${GL_LIBDIR}
    ${GLEW_LIBDIR}
)

add_subdirectory(FlatColorsCube)
add_subdirectory(VertexColorsCube)
add_subdirectory(TexturedCube)
