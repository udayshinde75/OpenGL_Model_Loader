#include <graphics/Camera.hpp>

// Constructor
Camera::Camera(float _cameraSpeed, float _mouseSensitivity, float _FOV, float _nearPlane, float _farPlane)
    :cameraSpeed(_cameraSpeed), mouseSensitivity(_mouseSensitivity), FOV(_FOV), nearPlane(_nearPlane), farPlane(_farPlane)
{
    UpdateVectors();
}

// Helper
void Camera::UpdateVectors()
{
    glm::vec3 _front;
    _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    _front.y = sin(glm::radians(pitch));
    _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(_front);

    // right = front x worldUp
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

    // up = right x front
    up = glm::normalize(glm::cross(right, front));
}

// Process mouse and keyboard
void Camera::ProcessMouse(const float deltaX, const float deltaY)
{
    yaw += deltaX * mouseSensitivity;
    pitch = glm::clamp(pitch + deltaY * mouseSensitivity, -89.0f, 89.0f);
    UpdateVectors();
}

void Camera::ProcessKeyboard(const glm::vec3 dir, float deltatime)
{
    glm::vec3 moveDir = glm::normalize(dir);
    position += moveDir * deltatime * cameraSpeed;
}

//Get matrices
glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix(const float aspectRatio) const
{
    return glm::perspective(glm::radians(FOV), aspectRatio, nearPlane, farPlane);
}

// Getters and setters
glm::vec3 Camera::GetCameraPositionVector() const
{
    return this->position;
}
glm::vec3 Camera::GetCameraFrontVector() const
{
    return this->front;
};
glm::vec3 Camera::GetCameraUpVector() const
{
    return this->up;
}
glm::vec3 Camera::GetCameraRightVector() const
{
    return this->right;
}
void Camera::SetCameraPositionVector(glm::vec3 position)
{
    this->position = position;
}
void Camera::SetCameraFrontVector(glm::vec3 front)
{
    this->front = front;
}
void Camera::SetCameraUpVector(glm::vec3 up)
{
    this->up = up;
}
void Camera::SetCameraRightVector(glm::vec3 right)
{
    this->right = right;
}
void Camera::SetCameraSpeed(float speed)
{
    this->cameraSpeed = speed;
}
void Camera::SetCameraMouseSensitivity(float sensitivity)
{
    this->mouseSensitivity = sensitivity;
}
void Camera::SetCameraFov(float FOV)
{
    this->FOV = FOV;
}
void Camera::SetCameraNearPlane(float nearPlane)
{
    this->nearPlane = nearPlane;
}
void Camera::SetCameraFarPlane(float farPlane)
{
    this->farPlane = farPlane;
}

float Camera::GetCameraFov() const
{
    return this->FOV;
}
