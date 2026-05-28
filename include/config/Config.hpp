#pragma once

#include <string>

// In trial, havent used much of it
namespace Config
{
    // Window
    namespace Window
    {
        inline constexpr int Width = 1280;
        inline constexpr int Height = 720;

        inline constexpr const char* Title = "OpenGL Renderer";

        inline constexpr bool StartFullscreen = false;
        inline constexpr bool EnableVSync = true;
    }

    // OpenGL
    namespace OpenGL
    {
        inline constexpr int MajorVersion = 4;
        inline constexpr int MinorVersion = 6;

        inline constexpr bool EnableDepthTest = true;
        inline constexpr bool EnableFaceCulling = false;
        inline constexpr bool EnableBlending = false;
    }

    // Camera
    namespace Camera
    {
        inline constexpr float FOV = 45.0f;

        inline constexpr float NearPlane = 0.1f;
        inline constexpr float FarPlane = 1000.0f;

        inline constexpr float MovementSpeed = 5.0f;
        inline constexpr float MouseSensitivity = 0.1f;
    }

    // Renderer
    namespace Renderer
    {
        inline constexpr float ClearColorR = 0.1f;
        inline constexpr float ClearColorG = 0.1f;
        inline constexpr float ClearColorB = 0.1f;
        inline constexpr float ClearColorA = 1.0f;
    }

    // Assets
    namespace Assets
    {
        inline const std::string ShaderDirectory ="assets/shaders/";

        inline const std::string TextureDirectory = "assets/textures/";

        inline const std::string ModelDirectory = "assets/models/";
    }
}