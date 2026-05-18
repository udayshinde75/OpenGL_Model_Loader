#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>

class Shader
{
    private:
    GLuint programID;
    mutable std::unordered_map<std::string, GLint> UniformCache;

    
};