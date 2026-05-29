#include <iostream>
#include <glm/detail/setup.hpp>
#include <glm/glm.hpp>
#include <glad/include/glad/glad.h>
#include <glfw/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <graphics/Camera.hpp>
#include <graphics/Renderer.hpp>
#include <graphics/Shader.hpp>
#include <graphics/Model.hpp>
#include <config/Config.hpp>
#include <core/Input.hpp>

static int SCR_WIDTH = 1280;
static int SCR_HEIGHT = 720;

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

static Camera camera(5.0f, 0.1f, 45.0f, 0.1f, 1000.0f);

static Renderer renderer;

void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height
);

void mouse_callback(
    GLFWwindow* window,
    double xPos,
    double yPos
);

void scroll_callback(
    GLFWwindow* window,
    double xOffset,
    double yOffset
);

void ProcessInput(GLFWwindow* window);

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);


    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Model Loader", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(
        (GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";

        glfwTerminate();

        return -1;
    }

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    renderer.EnableDepthTest(true);

    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    glfwSetCursorPosCallback(window,mouse_callback);

    glfwSetScrollCallback(window,scroll_callback);

    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

    Shader modelShader(
        "assets/shaders/vertex/cubeObjectVertexShader.glsl",
        "assets/shaders/fragment/cubeObjectFragmentShader.glsl"
    );

    Model backpackModel("assets/models/backpack/backpack.obj");

    glm::vec3 lightPosition(2.0f,0.0f,2.0f);

    // Render Loop
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    float fpsTimer = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());

        deltaTime = currentFrame - lastFrame;

        lastFrame = currentFrame;

        // Prevent crashes when window is minimized
        if (SCR_WIDTH == 0 || SCR_HEIGHT == 0)
        {
            glfwWaitEvents();
            continue;
        }

        // Calculate and display FPS every second
        fpsTimer += deltaTime;
        if (fpsTimer >= 1.0f)
        {
            float fps = 1.0f / deltaTime;
            std::cout << "FPS: " << fps << std::endl;
            fpsTimer = 0.0f;
        }

        ProcessInput(window);

        renderer.SetClearColor(0.1f,0.1f,0.1f,1.0f);

        renderer.ClearBuffers();

        modelShader.Use();

        lightPosition.x = cos(currentFrame) * 3.0f;
        lightPosition.z = sin(currentFrame) * 3.0f;

        glm::mat4 projection = camera.GetProjectionMatrix(static_cast<float>(SCR_WIDTH)/static_cast<float>(SCR_HEIGHT));

        glm::mat4 view = camera.GetViewMatrix();

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model,glm::vec3(0.0f,0.0f,0.0f));

        model = glm::scale(model,glm::vec3(1.0f));

        modelShader.SetMat4("projection", glm::value_ptr(projection));

        modelShader.SetMat4("view",glm::value_ptr(view));

        modelShader.SetMat4("model",glm::value_ptr(model));

        modelShader.SetVec3("viewPos",camera.GetCameraPositionVector().x,camera.GetCameraPositionVector().y,camera.GetCameraPositionVector().z);

        modelShader.SetVec3("light.position",lightPosition.x,lightPosition.y,lightPosition.z);

        modelShader.SetVec3("light.ambient",0.2f,0.2f,0.2f);

        modelShader.SetVec3("light.diffuse",1.0f,1.0f,1.0f);

        modelShader.SetVec3("light.specular",0.0f,0.0f,0.0f);

        modelShader.SetFloat("light.constant", 1.0f);
        modelShader.SetFloat("light.linear", 0.09f);
        modelShader.SetFloat("light.quadratic", 0.032f);

        backpackModel.Draw(modelShader);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow*,int width,int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;

    glViewport(0,0,width,height);
}

void mouse_callback(GLFWwindow*,double xPos,double yPos)
{
    Input::ProcessMouse(camera,xPos,yPos);
}

void scroll_callback(GLFWwindow*,double,double yOffset)
{
    Input::ProcessScroll(camera,yOffset);
}

void ProcessInput(GLFWwindow* window)
{
    Input::ProcessKeyboard(window,camera,deltaTime);
}