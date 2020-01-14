cmake_minimum_required(VERSION 2.8.12)
project(conan-opengl)

add_definitions("-std=c++11")

find_package(OpenGL REQUIRED)
include_directories(.)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB VERTEX_SHADER_FILES "*.vs")
file(COPY ${VERTEX_SHADER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

file(GLOB FRAGMENT_SHADER_FILES "*.fs")
file(COPY ${FRAGMENT_SHADER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)


add_executable(23_window 23_window.cpp)
target_link_libraries(23_window ${CONAN_LIBS} ${OPENGL_LIBRARY})

add_executable(24_triangle 24_triangle.cpp)
target_link_libraries(24_triangle ${CONAN_LIBS} ${OPENGL_LIBRARY})

add_executable(25_shaders 25_shaders.cpp)
target_link_libraries(25_shaders ${CONAN_LIBS} ${OPENGL_LIBRARY})
