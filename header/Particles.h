//
// Created by teo on 29. 8. 2026.
//

#ifndef MAZE_PARTICLES_H
#define MAZE_PARTICLES_H
#pragma once
#include <glm/glm.hpp>
#include <memory>
/**
 * @brief A single particle with position, velocity, and a limited lifetime.
 *
 * A particle counts down from `maxLife` to 0 via `ttl` as Update() is
 * called each frame, and is considered expired once `ttl` reaches 0.
 */
class Particles {
   public:
    Particles(glm::vec3 position, glm::vec3 velocity) : position(position), velocity(velocity) {};
    Particles(float x, float y, float z, glm::vec3 velocity)
        : position({x, y, z}), velocity(velocity) {};
    Particles(glm::vec3 position, float x, float y, float z)
        : position(position), velocity({x, y, z}) {};
    Particles(float px, float py, float pz, float vx, float vy, float vz)
        : position({px, py, pz}), velocity({vx, vy, vz}) {};

    /// Advances the particle by `deltaTime`: moves it by velocity and
    /// counts down `ttl`, setting `alive = false` once it expires.
    void Update(float deltaTime);
    /// Getters Setters
    void setPos(glm::vec3 pos) { position = pos; };
    void setVelocity(glm::vec3 velocity) { this->velocity = velocity; };
    glm::vec3 getPosition() { return position; };

    bool alive = false;  /// Whether the particle is currently active/should be rendered

   private:
    glm::vec3 position;
    glm::vec3 velocity;
    float maxLife = 4.0f;
    float ttl = maxLife;
};

#endif  // MAZE_PARTICLES_H
