cmake_minimum_required(VERSION 3.10)

project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(OpenGLProject
    src/main.cpp
    external/glad/src/glad.c
)

target_include_directories(OpenGLProject PRIVATE external)

target_link_libraries(OpenGLProject
    glfw
)