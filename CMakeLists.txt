cmake_minimum_required(VERSION 3.10)

project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

add_executable(OpenGLProject
    src/main.cpp
    external/glad/src/glad.c
    src/graphics/Camera.cpp
    src/graphics/Shader.cpp
    src/graphics/Renderer.cpp
    src/graphics/Mesh.cpp
    src/graphics/Model.cpp
    src/core/Input.cpp
    src/graphics/Texture.cpp
)

target_include_directories(OpenGLProject PRIVATE
    external
    include
    src
)

target_link_libraries(OpenGLProject
    glfw
    glm::glm
    assimp::assimp
)