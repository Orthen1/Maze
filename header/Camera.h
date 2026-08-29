//
// Created by teo on 26. 7. 2026.
//

#ifndef MAZE_CAMERA_H
#define MAZE_CAMERA_H
#pragma once
#include <glad/glad.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "BoundingBox.h"

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 1.0f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

class Camera {
   public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    bool collisionsOn = true;
    bool fly = false;
    BoundingBox bounding_box;

    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH,
        BoundingBox box = BoundingBox(0.0f, 0.0f, 0.0f, 0.2f, 1.0f, 0.2f)
    );
    Camera(
        float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch,
        BoundingBox box
    );

    glm::mat4 getViewMatrix() const { return glm::lookAt(Position, Position + Front, Up); }

    void processKeyboard(CameraMovement direction, float deltaTime);
    void updateCameraBoundingBox();
    void processMouse(float xoffset, float yoffset, GLboolean constrainPitch = true);

   private:
    void updateCameraVectors();
};

#endif  // MAZE_CAMERA_H
