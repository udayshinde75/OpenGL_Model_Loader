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

// ======================================================
// Global State
// ======================================================

static int SCR_WIDTH = 1280;
static int SCR_HEIGHT = 720;

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

static Camera camera(
    5.0f,     // movement speed
    0.1f,     // mouse sensitivity
    45.0f,    // FOV
    0.1f,     // near plane
    1000.0f   // far plane
);

static Renderer renderer;

// ======================================================
// Callback Declarations
// ======================================================

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

// ======================================================
// Main
// ======================================================

int main()
{
    // ==================================================
    // GLFW Initialization
    // ==================================================

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );

#ifdef __APPLE__

    glfwWindowHint(
        GLFW_OPENGL_FORWARD_COMPAT,
        GL_TRUE
    );

#endif

    // ==================================================
    // Create Window
    // ==================================================

    GLFWwindow* window =
        glfwCreateWindow(
            SCR_WIDTH,
            SCR_HEIGHT,
            "Model Loader",
            nullptr,
            nullptr
        );

    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    // ==================================================
    // Load GLAD
    // ==================================================

    if (!gladLoadGLLoader(
        (GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";

        glfwTerminate();

        return -1;
    }

    // ==================================================
    // OpenGL State
    // ==================================================

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    renderer.EnableDepthTest(true);

    // ==================================================
    // Callbacks
    // ==================================================

    glfwSetFramebufferSizeCallback(
        window,
        framebuffer_size_callback
    );

    glfwSetCursorPosCallback(
        window,
        mouse_callback
    );

    glfwSetScrollCallback(
        window,
        scroll_callback
    );

    glfwSetInputMode(
        window,
        GLFW_CURSOR,
        GLFW_CURSOR_DISABLED
    );

    // ==================================================
    // Shader Creation
    // ==================================================

    Shader modelShader(
        "/Users/udayshinde/Desktop/OpenGLProjects/OpenGL_Model_Loader/assets/shaders/vertex/cubeObjectVertexShader.glsl",

        "/Users/udayshinde/Desktop/OpenGLProjects/OpenGL_Model_Loader/assets/shaders/fragment/cubeObjectFragmentShader.glsl"
    );

    // ==================================================
    // Model Loading
    // ==================================================

    Model backpackModel(
        "/Users/udayshinde/Desktop/OpenGLProjects/OpenGL_Model_Loader/assets/models/backpack/backpack.obj"
    );

    // ==================================================
    // Lighting
    // ==================================================

    glm::vec3 lightPosition(
        2.0f,
        4.0f,
        2.0f
    );

    // ==================================================
    // Render Loop
    // ==================================================
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    float fpsTimer = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        // ==============================================
        // Delta Time
        // ==============================================

        float currentFrame =
            static_cast<float>(glfwGetTime());

        deltaTime =
            currentFrame - lastFrame;

        lastFrame = currentFrame;

        // Calculate and display FPS every second
        fpsTimer += deltaTime;
        if (fpsTimer >= 1.0f)
        {
            float fps = 1.0f / deltaTime;
            std::cout << "FPS: " << fps << std::endl;
            fpsTimer = 0.0f;
        }

        // ==============================================
        // Input
        // ==============================================

        ProcessInput(window);

        // ==============================================
        // Clear
        // ==============================================

        renderer.SetClearColor(
            0.1f,
            0.1f,
            0.1f,
            1.0f
        );

        renderer.ClearBuffers();

        // ==============================================
        // Shader
        // ==============================================

        modelShader.Use();

        // Animate light around the object in the XZ plane.
        lightPosition.x = cos(currentFrame) * 3.0f;
        lightPosition.z = sin(currentFrame) * 3.0f;

        // ==============================================
        // Projection
        // ==============================================

        glm::mat4 projection =
            camera.GetProjectionMatrix(
                static_cast<float>(SCR_WIDTH) /
                static_cast<float>(SCR_HEIGHT)
            );

        // ==============================================
        // View
        // ==============================================

        glm::mat4 view =
            camera.GetViewMatrix();

        // ==============================================
        // Model Matrix
        // ==============================================

        glm::mat4 model =
            glm::mat4(1.0f);

        model =
            glm::translate(
                model,
                glm::vec3(
                    0.0f,
                    -1.75f,
                    0.0f
                )
            );

        model =
            glm::scale(
                model,
                glm::vec3(1.0f)
            );

        // ==============================================
        // Uniforms
        // ==============================================

        modelShader.SetMat4(
            "projection",
            glm::value_ptr(projection)
        );

        modelShader.SetMat4(
            "view",
            glm::value_ptr(view)
        );

        modelShader.SetMat4(
            "model",
            glm::value_ptr(model)
        );

        modelShader.SetVec3(
            "viewPos",
            camera.GetCameraPositionVector().x,
            camera.GetCameraPositionVector().y,
            camera.GetCameraPositionVector().z
        );

        modelShader.SetVec3(
            "light.position",
            lightPosition.x,
            lightPosition.y,
            lightPosition.z
        );

        modelShader.SetVec3(
            "light.ambient",
            0.2f,
            0.2f,
            0.2f
        );

        modelShader.SetVec3(
            "light.diffuse",
            1.0f,
            1.0f,
            1.0f
        );

        modelShader.SetVec3(
            "light.specular",
            1.0f,
            1.0f,
            1.0f
        );

        modelShader.SetFloat("light.constant", 1.0f);
        modelShader.SetFloat("light.linear", 0.09f);
        modelShader.SetFloat("light.quadratic", 0.032f);

        // ==============================================
        // Draw Model
        // ==============================================

        backpackModel.Draw(modelShader);

        // ==============================================
        // Swap Buffers
        // ==============================================

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // ==================================================
    // Cleanup
    // ==================================================

    glfwTerminate();

    return 0;
}

// ======================================================
// Callbacks
// ======================================================

void framebuffer_size_callback(
    GLFWwindow*,
    int width,
    int height
)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;

    glViewport(
        0,
        0,
        width,
        height
    );
}

void mouse_callback(
    GLFWwindow*,
    double xPos,
    double yPos
)
{
    Input::ProcessMouse(
        camera,
        xPos,
        yPos
    );
}

void scroll_callback(
    GLFWwindow*,
    double,
    double yOffset
)
{
    Input::ProcessScroll(
        camera,
        yOffset
    );
}

void ProcessInput(GLFWwindow* window)
{
    Input::ProcessKeyboard(
        window,
        camera,
        deltaTime
    );
}