cmake_minimum_required(VERSION 2.8.12)
project(conan-opengl)

add_definitions("-std=c++11")

find_package(OpenGL REQUIRED)
include_directories(.)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(23_window 23_window.cpp)
target_link_libraries(23_window ${CONAN_LIBS} ${OPENGL_LIBRARY})

add_executable(24_triangle 24_triangle.cpp)
target_link_libraries(24_triangle ${CONAN_LIBS} ${OPENGL_LIBRARY})
