//
// Created by teo on 26. 7. 2026.
//

#ifndef MAZE_CAMERA_H
#define MAZE_CAMERA_H
#pragma once
#include <glad/glad.h>


#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

struct BoundingBox {
    float minX, minY, minZ , maxX, maxY,maxZ;
    BoundingBox(float minX, float minY, float minZ, float maxX ,float maxY, float maxZ) {
        this->minX = minX;
        this->minY = minY;
        this->minZ = minZ;
        this->maxX = maxX;
        this->maxY = maxY;
        this->maxZ = maxZ;
    }

    bool intersec(BoundingBox &other) const {
        return  minX <= other.maxX && maxX >= other.minX &&
                minY <= other.maxY && maxY >= other.minY &&
                minZ <= other.maxZ && maxZ >= other.minZ;
    }

    std::vector<float> boxToVertex() {
        return {
            minX,minY,minZ,
            maxX,minY,minZ,
            maxX,minY,maxZ,
            minX,minY,maxZ,

            minX,maxY,minZ,
            maxX,maxY,minZ,
            maxX,maxY,maxZ,
            minX,maxY,maxZ
        };

    };

};

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};


const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;



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
    bool fly = false;
    BoundingBox bounding_box;


    Camera( glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH , BoundingBox box = BoundingBox(0.0f,0.0f,0.0f,0.2f,1.0f,0.2f));
    Camera( float posX,float posY, float posZ,float upX, float upY, float upZ,float yaw, float pitch, BoundingBox box );


    glm::mat4 getViewMatrix() {
        return glm::lookAt( Position, Position + Front, Up );
    }

    void processKeyboard(CameraMovement direction, float deltaTime);
    void updateCameraBoundingBox();
    void processMouse(float xoffset, float yoffset, GLboolean constrainPitch = true);

private:
    void updateCameraVectors();





};



#endif //MAZE_CAMERA_H
