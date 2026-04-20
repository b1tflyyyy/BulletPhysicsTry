cmake_minimum_required(VERSION 3.22)
project(MyImpl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(myimpl
    src/main.cpp
    src/ProjectileSimulation.cpp
    src/Renderer.cpp
)

target_include_directories(myimpl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(myimpl
    PRIVATE
        OpenGL::GL
        glfw
        GLEW::glew
)

target_compile_options(myimpl PRIVATE -Wall -Wextra -Wpedantic)
