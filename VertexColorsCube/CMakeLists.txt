cmake_minimum_required(VERSION 2.8)

project(VertexColorsCube)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    ${GLEW_LIBRARIES}
    ${SDL2_LIBRARY}
)
