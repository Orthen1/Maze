//
// Created by teo on 29. 8. 2026.
//

#ifndef MAZE_PARTICLESPOOL_H
#define MAZE_PARTICLESPOOL_H
#pragma once
#include <memory>

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
    uint VAO;
    uint VBO;
};

#endif  // MAZE_PARTICLESPOOL_H
