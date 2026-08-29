//
// Created by teo on 29. 8. 2026.
//

#ifndef MAZE_PARTICLES_H
#define MAZE_PARTICLES_H
#pragma once
#include <glm/glm.hpp>
#include <memory>
class Particles {
   public:
    Particles(glm::vec3 position, glm::vec3 velocity) : position(position), velocity(velocity) {};
    Particles(float x, float y, float z, glm::vec3 velocity)
        : position({x, y, z}), velocity(velocity) {};
    Particles(glm::vec3 position, float x, float y, float z)
        : position(position), velocity({x, y, z}) {};
    Particles(float px, float py, float pz, float vx, float vy, float vz)
        : position({px, py, pz}), velocity({vx, vy, vz}) {};
    void setPos(glm::vec3 pos) { position = pos; };
    void setVelocity(glm::vec3 velocity) { this->velocity = velocity; };
    glm::vec3 getPosition() { return position; };
    void Update(float deltaTime);
    bool alive = false;

   private:
    glm::vec3 position;
    glm::vec3 velocity;
    float maxLife = 1.0f;
    float ttl = maxLife;
};

#endif  // MAZE_PARTICLES_H
