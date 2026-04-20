cmake_minimum_required(VERSION 3.22)
project(WithLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(BulletPhysics)

add_executable(withlib
    src/main.cpp
    src/ProjectileSimulation.cpp
    src/Renderer.cpp
)

target_include_directories(withlib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(withlib
    PRIVATE
        BulletPhysics
        OpenGL::GL
        glfw
        GLEW::glew
)

target_compile_options(withlib PRIVATE -Wall -Wextra -Wpedantic)
