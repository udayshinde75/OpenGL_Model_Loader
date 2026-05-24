#pragma once

#include <glm/vec4.hpp>

class Model;
class Shader;
class Camera;

class Renderer
{
    private:

    glm::vec4 clearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

    bool depthTestEnabled = true;
    bool blendingEnabled = false;
    bool faceCullingEnabled = false;

    public:

    Renderer();
    ~Renderer() = default;

    // Frame Operations
    void BeginFrame() const;
    void EndFrame() const;

    void ClearColorBuffer() const;
    void ClearDepthBuffer() const;
    void ClearBuffers() const;

    // Render State
    void SetClearColor(float r, float g, float b, float a);

    void EnableDepthTest(bool enable);
    void EnableBlending(bool enable);
    void EnableFaceCulling(bool enable);

    // Rendering
    void DrawModel(const Model& model, const Shader& shader) const;
};