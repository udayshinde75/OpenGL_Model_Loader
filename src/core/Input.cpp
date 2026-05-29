#include "core/Input.hpp"
#include "graphics/Camera.hpp"

#include <glfw/glfw3.h>

bool Input::firstMouse = true;

float Input::lastMouseX = 0.0f;
float Input::lastMouseY = 0.0f;

void Input::ProcessKeyboard(GLFWwindow* window, Camera& camera, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(camera.GetCameraFrontVector(),deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(-camera.GetCameraFrontVector(),deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(-camera.GetCameraRightVector(),deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(camera.GetCameraRightVector(),deltaTime);
    }
}

void Input::ProcessMouse(Camera& camera, double mouseX, double mouseY)
{
    if (firstMouse)
    {
        lastMouseX = static_cast<float>(mouseX);
        lastMouseY = static_cast<float>(mouseY);

        firstMouse = false;
    }

    float deltaX = static_cast<float>(mouseX) - lastMouseX;

    float deltaY = lastMouseY - static_cast<float>(mouseY);

    lastMouseX = static_cast<float>(mouseX);
    lastMouseY = static_cast<float>(mouseY);

    camera.ProcessMouse(deltaX, deltaY);
}

void Input::ProcessScroll(Camera& camera, double offsetY)
{
    float fov = camera.GetCameraFov();
    fov -= static_cast<float>(offsetY);
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 90.0f) fov = 90.0f;
    camera.SetCameraFov(fov);
}

void Input::SetFirstMouse(bool value)
{
    firstMouse = value;
}

bool Input::IsFirstMouse()
{
    return firstMouse;
}