//
// Created by teo on 29. 8. 2026.
//

#ifndef MAZE_PARTICLESPOOL_H
#define MAZE_PARTICLESPOOL_H
#pragma once
#include <memory>
#include <vector>

#include "Particles.h"

class ParticlesPool {
   public:
    ParticlesPool(int max);
    void Spawn(glm::vec3 position, glm::vec3 velocity);
    void Update(float deltaTime);
    void Pour(glm::vec3 pos, int particlesPerFrame, float deltaTime);
    void Render();

   private:
    std::unique_ptr<std::vector<Particles>> particlesPool;
    int maxSize;
    int currentAlive;
    unsigned int VAO;
    unsigned int VBO;
};

#endif  // MAZE_PARTICLESPOOL_H
