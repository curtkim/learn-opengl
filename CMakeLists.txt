cmake_minimum_required(VERSION 2.8.12)
project(conan-opengl)

add_definitions("-std=c++11")

find_package(OpenGL REQUIRED)
include_directories(.)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB SHADER_FILES "*.shader")
file(COPY ${SHADER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

foreach(APP 23_window 24_triangle 25_shaders 26_textures 27_transformations 28_coordinates)
    add_executable(${APP} ${APP}.cpp)
    target_link_libraries(${APP} ${CONAN_LIBS} ${OPENGL_LIBRARY})
endforeach()