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

/// Directions a camera can be moved in via keyboard input.
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};
// Default camera construction values
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 1.0f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

/**
 * @brief A free-fly / FPS camera with Euler-angle orientation and
 *        optional collision detection via a bounding box.
 *
 * Tracks position and orientation (yaw/pitch), derives Front/Right/Up basis
 * vectors from those angles, and exposes helpers to move via keyboard/mouse
 * input and produce a view matrix for rendering.
 */
class Camera {
   public:
    glm::vec3 Position;

    glm::vec3 Front;    /// Direction the camera is looking, derived from Yaw/Pitch
    glm::vec3 Up;       /// Camera-local up vector, derived from Front and WorldUp
    glm::vec3 Right;    /// Camera-local right vector, derived from Front and WorldUp
    glm::vec3 WorldUp;  /// World-space up reference used to compute Right/Up

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    // NoClip debug flags
    bool collisionsOn = true;
    bool fly = false;
    BoundingBox bounding_box;

    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH
    );
    Camera(
        float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch
    );

    /// Builds the view matrix for the camera's current position/orientation.
    glm::mat4 getViewMatrix() const { return glm::lookAt(Position, Position + Front, Up); }

    /// Applies mouse movement to yaw/pitch and updates the camera's
    /// basis vectors accordingly.
    void processMouse(float xoffset, float yoffset, GLboolean constrainPitch = true);

    void processKeyboard(CameraMovement direction, float deltaTime);
    /// Recomputes bounding_box's position to follow the camera's current Position.
    void updateCameraBoundingBox();

   private:
    void updateCameraVectors();
};

#endif  // MAZE_CAMERA_H
