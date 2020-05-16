cmake_minimum_required(VERSION 3.15)
project(conan-opengl)

add_definitions("-std=c++17")

find_package(OpenGL REQUIRED)
include_directories(.)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

#file(GLOB SHADER_FILES "*.shader")
#file(COPY ${SHADER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

foreach(APP
        23_window 24_triangle 25_shaders 26_textures 27_transformations 28_coordinates 29_camera
        31_color 32_basic_light 33_material 34_lighting_map 35_light_caster 36_multi_light
        43_model_load
        55_framebuffer
        91_geometry_shader_houses)
    add_executable(${APP} ${APP}.cpp)
    target_link_libraries(${APP} ${CONAN_LIBS} ${OPENGL_LIBRARY} X11)
endforeach()

foreach(APP
    23_window 25_shaders)
    add_executable(modern_${APP} moderngl/${APP}.cpp)
    target_link_libraries(modern_${APP} ${CONAN_LIBS} ${OPENGL_LIBRARY} X11)
endforeach()