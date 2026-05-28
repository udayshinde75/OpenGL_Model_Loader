#pragma once

#include <string>
#include <unordered_map>
#include <glad/include/glad/glad.h>

class Shader
{
    private:
    GLuint programID;
    mutable std::unordered_map<std::string, GLint> UniformCache;

    public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Use() const;

    GLuint GetProgramID() const;

    // Setters
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;
    void SetMat4(const std::string& name, const float* matrix) const;

    private:
    // Helpers
    std::string ReadFile(const std::string& filepath);
    GLuint CompileShader(GLenum type, const std::string& source);
    void CheckCompileErrors(GLuint object, const std::string& type);
    GLint GetUniformLocation(const std::string& name) const;
};