#include <iostream>
#include <glm/detail/setup.hpp>
#include <glm/glm.hpp>
#include <glad/include/glad/glad.h>
#include <glfw/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <include/graphics/Camera.hpp>
#include <include/graphics/Renderer.hpp>
#include <include/graphics/Shader.hpp>
#include <include/graphics/Model.hpp>
#include <include/config/Config.hpp>
#include <include/core/Input.hpp>


glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
Model backPackModel(Config::Assets::ModelDirectory + "backpack/backpack.obj");
Camera camera(0.5f, 10.0f, 45.0f, 1.0f, 1000.0f);
Renderer renderer;
float deltaTime;
float lastFrame;
int SCR_WIDTH;
int SCR_HEIGHT;

/// Callbacks ---- (start)
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, GLdouble xPos, GLdouble yPos);
void scroll_callback(GLFWwindow* window, GLdouble xOffset, GLdouble yOffset);
void ProcessInput(GLFWwindow* window);
/// Callbacks ---- (end)

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Point light with decreasing brightness", monitor, nullptr);
    glfwMakeContextCurrent(window);

    SCR_WIDTH = mode->width;
    SCR_HEIGHT = mode->height;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    renderer.EnableDepthTest(true);

    // Compile shader
    std::cout << "Compiling cube objects Program --- (start)" << std::endl;
    Shader cubeObjectShader(Config::Assets::ShaderDirectory + "fragment/cubeObjectVertexShader.glsl", Config::Assets::ShaderDirectory + "fragment/cubeObjectFragmentShader.glsl");
    std::cout << "Compiling cube objetcs Program --- (end)" << std::endl;

    std::cout << "Compiling lighting cube program -- (start)" << std::endl;
    Shader lightCubeShader(Config::Assets::ShaderDirectory + "fragment/lightCubeVertexShader.glsl", Config::Assets::ShaderDirectory + "fragment/lightCubeFragmentShader.glsl");
    std::cout << "Compiling lighting cube program -- (end)" << std::endl;

    std::cout << "Generating ShaderProgramSky --- (start)" << std::endl;
    Shader lightCubeShader(Config::Assets::ShaderDirectory + "fragment/cubeMapVertexShader.glsl", Config::Assets::ShaderDirectory + "fragment/cubeMapFragmentShader.glsl");
    std::cout << "Generating ShaderProgramSky --- (end)" << std::endl;

    float skyboxVertices[] = {
        -1,  1, -1, -1, -1, -1,  1, -1, -1,  1, -1, -1,  1,  1, -1, -1,  1, -1,
        -1, -1,  1, -1, -1, -1, -1,  1, -1, -1,  1, -1, -1,  1,  1, -1, -1,  1,
         1, -1, -1,  1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1,  1, -1, -1,
        -1, -1,  1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1,  1, -1, -1,  1,
        -1,  1, -1,  1,  1, -1,  1,  1,  1,  1,  1,  1, -1,  1,  1, -1,  1, -1,
        -1, -1, -1, -1, -1,  1,  1, -1, -1,  1, -1, -1, -1, -1,  1,  1, -1,  1
    };

    // Skybox VAO
    //GLuint skyVAO, skyVBO;
    //glGenVertexArrays(1, &skyVAO);
    //glGenBuffers(1, &skyVBO);

    //glBindVertexArray(skyVAO);
    //glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    //glBindVertexArray(0);

    std::vector<std::string> faces = {
        "/Users/udayshinde/Desktop/OpenGLWindow/Assets/right.jpg",
        "/Users/udayshinde/Desktop/OpenGLWindow/Assets/left.jpg",
        "/Users/udayshinde/Desktop/OpenGLWindow/Assets/top.jpg",
        "/Users/udayshinde/Desktop/OpenGLWindow/Assets/bottom.jpg",
        "/Users/udayshinde/Desktop/OpenGLWindow/Assets/front.jpg",
        "/Users/udayshinde/Desktop/OpenGLWindow/Assets/back.jpg"
        };

    cubeObjectShader.SetVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
    
    GLint LightPosition = glGetUniformLocation(cubeObjectProgram, "light.position");
    std::cout << "LightPosition : " << LightPosition << std::endl;
    GLint ViewPosition = glGetUniformLocation(cubeObjectProgram, "viewPos");
    std::cout << "ViewPosition : " << ViewPosition << std::endl;
    GLint LightAmbient = glGetUniformLocation(cubeObjectProgram, "light.ambient");
    std::cout << "LightAmbient : " << LightAmbient << std::endl;
    GLint LightDiffuse = glGetUniformLocation(cubeObjectProgram, "light.diffuse");
    std::cout << "LightDiffuse : " << LightDiffuse << std::endl;
    GLint LightSpecular = glGetUniformLocation(cubeObjectProgram, "light.specular");
    std::cout << "LightSpecular : " << LightSpecular << std::endl;
    GLint LightConstant = glGetUniformLocation(cubeObjectProgram, "light.constant");
    std::cout << "LightConstant : " << LightSpecular << std::endl;
    GLint LightLinear = glGetUniformLocation(cubeObjectProgram, "light.linear");
    std::cout << "LightLinear : " << LightSpecular << std::endl;
    GLint LightQuadratic = glGetUniformLocation(cubeObjectProgram, "light.quadratic");
    std::cout << "LightQuadratic : " << LightSpecular << std::endl;
    GLint MaterialShininess = glGetUniformLocation(cubeObjectProgram, "material.shininess");
    std::cout << "MaterialShininess : " << MaterialShininess << std::endl;
    GLint MaterialDiffuse = glGetUniformLocation(cubeObjectProgram, "material.diffuse");
    std::cout << "MaterialDiffuse : " << MaterialDiffuse << std::endl;
    glUniform1i(glGetUniformLocation(cubeObjectProgram, "material.diffuse"), 0);
    GLint MaterialSpecular = glGetUniformLocation(cubeObjectProgram, "material.specular");
    std::cout << "MaterialSpecular : " << MaterialSpecular << std::endl;
    glUniform1i(glGetUniformLocation(cubeObjectProgram, "material.specular"), 1);
    GLint LightShaderProjection = glGetUniformLocation(cubeObjectProgram, "projection");
    std::cout << "LightShaderProjection : " << LightShaderProjection << std::endl;
    GLint LightShaderView = glGetUniformLocation(cubeObjectProgram, "view");
    std::cout << "LightShaderView : " << LightShaderView << std::endl;
    GLint LightShaderModel = glGetUniformLocation(cubeObjectProgram, "model");
    std::cout << "LightShaderModel : " << LightShaderModel << std::endl;

    float currentFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(cubeObjectProgram);
        glUniform3fv(LightPosition,1, glm::value_ptr(lightPos));
        glUniform3fv(ViewPosition,1,  glm::value_ptr(camera.Position));

        glUniform3fv(LightAmbient, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
        glUniform3fv(LightDiffuse, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniform3fv(LightSpecular, 1, glm::value_ptr(glm::vec3(2.0f, 2.0f, 2.0f)));
        glUniform1f(LightConstant, 1.0f);
        glUniform1f(LightLinear, 0.045f);
        glUniform1f(LightQuadratic, 0.0075f);

        glUniform1f(MaterialShininess, 512.0f);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 1000.0f);
        glm::mat4 view = getCameraViewMatrix(&camera);
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(LightShaderProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(LightShaderView, 1, GL_FALSE, glm::value_ptr(view));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(cubeVAO);

        for (unsigned int i = 0; i < 6; i++) {

            model = glm::mat4(1.0f);
            const float angle = 5.0f * (i+1) * glfwGetTime();
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));

            model = glm::translate(model, cubePositions[i]);


            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            glUniformMatrix4fv(LightShaderModel, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glUseProgram(lightCubeProgram);
        glUniformMatrix4fv(glGetUniformLocation(lightCubeProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightCubeProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightCubeProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(lightCubeProgram, "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f)));

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        if (true) {
            glDepthMask(GL_FALSE);
            glDepthFunc(GL_LEQUAL);
            glUseProgram(ShaderProgramSky);

            glm::mat4 skyView = glm::mat4(glm::mat3(view)); // remove translation
            glUniformMatrix4fv(glGetUniformLocation(ShaderProgramSky,"view"),1,GL_FALSE,glm::value_ptr(skyView));
            glUniformMatrix4fv(glGetUniformLocation(ShaderProgramSky,"projection"),1,GL_FALSE,glm::value_ptr(projection));

            glBindVertexArray(skyVAO);
            glUniform1i(glGetUniformLocation(ShaderProgramSky, "skybox"), 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTex);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindTexture(GL_TEXTURE_2D, 0);

            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "OpenGL error: 0x" << std::hex << err << std::dec << std::endl;
        }
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int, int)
{
    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

void mouse_callback(GLFWwindow* window, const double xPos, const double yPos)
{
    Input::ProcessMouse(camera, xPos, yPos);
}

void scroll_callback(GLFWwindow*, double, const double yOffset)
{
    Input::ProcessScroll(camera, yOffset);
}

void ProcessInput(GLFWwindow *window)
{
    Input::ProcessKeyboard(window, camera, deltaTime);
}