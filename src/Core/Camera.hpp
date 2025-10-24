//
// Created by Bisher Almasri on 2025-10-24.
//
#pragma once
#include "glad/glad.h"

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float ROLL = 0.0f;
// constexpr float FOV         =  90.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UPWARD,
    DOWNWARD
};

class Camera
{
public:
    glm::vec3 Position{};
    glm::vec3 Up{};
    glm::vec3 Right{};
    glm::vec3 Front;
    glm::vec3 WorldUp{};
    float Yaw;
    float Pitch;
    float Roll;
    // float Fov;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructer with vec
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
                    float pitch = PITCH, float roll = ROLL);

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw,
           float pitch, float roll);

    glm::mat4 GetViewMatrix() const;

    void ProcessKeyboardInput(Camera_Movement direction, float deltaTime);

    void ProcessMouseInput(float xOffset, float yOffset, GLboolean constrainPitch);

    void ProcessMouseScroll(float yOffset);

private:
    void updateCameraVectors();
};
