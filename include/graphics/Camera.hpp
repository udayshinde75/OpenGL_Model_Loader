#pragma once

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <glm/detail/setup.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
    private:
    glm::vec3 position = glm::vec3(0.0f, 5.0f, -5.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f,-1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    float yaw = -90.0f;
    float pitch = 0.0f;

    float cameraSpeed;
    float mouseSensitivity;
    float FOV;
    float nearPlane;
    float farPlane;

    private:
    void UpdateVectors();

    public:
    Camera(float _cameraSpeed, float _mouseSensitivity, float _FOV, float _nearPlane, float _farPlane);
    ~Camera() = default;
    void ProcessMouse(const float deltaX, const float deltaY);
    void ProcessKeyboard(const glm::vec3 dir, float deltaTime);
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(const float aspectRatio) const;
    float GetCameraFov() const;
    glm::vec3 GetCameraPositionVector() const;
    glm::vec3 GetCameraFrontVector() const;
    glm::vec3 GetCameraUpVector() const;
    glm::vec3 GetCameraRightVector() const;
    void SetCameraPositionVector(glm::vec3 position);
    void SetCameraFrontVector(glm::vec3 front);
    void SetCameraUpVector(glm::vec3 up);
    void SetCameraRightVector(glm::vec3 right);
    void SetCameraSpeed(float speed);
    void SetCameraMouseSensitivity(float sensitivity);
    void SetCameraFov(float FOV);
    void SetCameraNearPlane(float nearPlane);
    void SetCameraFarPlane(float farPlane);
};