//
// Created by teo on 26. 7. 2026.
//

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM),
      bounding_box(
          position.x - 0.2f, 0.0f, position.z - 0.2f, position.x + 0.2f, 2.0f, position.z + 0.2f
      ) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(
    float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch
)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY),
      Zoom(ZOOM),
      bounding_box(posX - 0.2f, 0.0f, posZ - 0.2f, posX + 0.2f, 2.0f, posZ + 0.2f) {
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD) {
        Position += Front * velocity;
    }
    if (direction == BACKWARD) {
        Position -= Front * velocity;
    }
    if (direction == LEFT) {
        Position -= Right * velocity;
    }
    if (direction == RIGHT) {
        Position += Right * velocity;
    }
    if (!fly) {
        Position.y = 0.0f;
    }
}

void Camera::processMouse(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Clamp pitch just short of +/-90 degrees to avoid the view flipping
    // upside down (gimbal-lock-style artifact) at the poles.
    if (constrainPitch) {
        if (Pitch > 89.0f) {
            Pitch = 89.0f;
        }
        if (Pitch < -89.0f) {
            Pitch = -89.0f;
        }
    }
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // spherical-to-Cartesian conversion from yaw/pitch to a unit
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // Re-derive the orthonormal basis: Right is perpendicular to Front and
    // world-up, and Up is perpendicular to both.
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
void Camera::updateCameraBoundingBox() {
    // Recenter the bounding box on the camera's current position.
    bounding_box.minX = Position.x - 0.15f;
    bounding_box.minZ = Position.z - 0.15f;
    bounding_box.minY = Position.y - 0.2f;
    bounding_box.maxX = Position.x + 0.15f;
    bounding_box.maxZ = Position.z + 0.15f;
    bounding_box.maxY = Position.y + 0.2f;
}
