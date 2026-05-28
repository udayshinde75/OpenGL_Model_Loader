#include "graphics/Renderer.hpp"
#include "graphics/Model.hpp"
#include "graphics/Shader.hpp"

#include <glad/include/glad/glad.h>

Renderer::Renderer()
{

}

void Renderer::BeginFrame() const
{
    ClearBuffers();
}

void Renderer::EndFrame() const
{
    // For future learning.
    // Later:
    // - batching
    // - command queue flushing
    // - post processing
}

void Renderer::ClearColorBuffer() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::ClearDepthBuffer() const
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::ClearBuffers() const
{
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    clearColor = glm::vec4(r, g, b, a);

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

void Renderer::EnableDepthTest(bool enable)
{
    depthTestEnabled = enable;

    if (enable)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

void Renderer::EnableBlending(bool enable)
{
    blendingEnabled = enable;

    if (enable)
    {
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}

void Renderer::EnableFaceCulling(bool enable)
{
    faceCullingEnabled = enable;

    if (enable)
    {
        glEnable(GL_CULL_FACE);

        glCullFace(GL_BACK);

        glFrontFace(GL_CCW);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}

void Renderer::DrawModel(const Model& model, const Shader& shader) const
{
    shader.Use();

    model.Draw(shader);
}