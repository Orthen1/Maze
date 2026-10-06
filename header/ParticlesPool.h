//
// Created by teo on 29. 8. 2026.
//

#ifndef MAZE_PARTICLESPOOL_H
#define MAZE_PARTICLESPOOL_H
#pragma once
#include <memory>
#include <vector>

#include "Particles.h"

/**
 * @brief A fixed-size, pre-allocated pool of particles.
 *
 *Particle object pool so we can save some time in render if we wanna use them at multiple times.
 *Owns the GL VAO/VBO used to render all particles in one draw call.
 */
class ParticlesPool {
   public:
    ParticlesPool(int max);
    /// Spawns a single new particle at `position` with `velocity`, reusing
    /// a dead slot if the pool is full.
    void Spawn(glm::vec3 position, glm::vec3 velocity);
    void Update(float deltaTime);

    /// Spawns a steady stream of particles
    void Pour(glm::vec3 pos, int particlesPerFrame, float deltaTime);
    void Elipse(glm::vec3 pos, float deltaTime);
    void Render();

   private:
    std::unique_ptr<std::vector<Particles>> particlesPool;
    int maxSize;
    int currentAlive;
    unsigned int VAO;
    unsigned int VBO;
};

#endif  // MAZE_PARTICLESPOOL_H
