#pragma once

struct GLFWwindow;

class Camera;

class Input
{
    public:

    static void ProcessKeyboard(GLFWwindow* window, Camera& camera, float deltaTime);

    static void ProcessMouse(Camera& camera, double mouseX, double mouseY);

    static void ProcessScroll(Camera& camera, double offsetY);

    static void SetFirstMouse(bool value);

    static bool IsFirstMouse();

    private:

    static bool firstMouse;

    static float lastMouseX;
    static float lastMouseY;
};