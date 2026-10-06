//
// Created by teo on 29. 8. 2026.
//

#include "../header/ParticlesPool.h"

#include <glad/glad.h>

ParticlesPool::ParticlesPool(int max) : particlesPool(std::make_unique<std::vector<Particles>>()) {
    maxSize = max;
    for (int i = 0; i < max; i++) {
        particlesPool->push_back({0, 0, 0, 0, 0, 0});
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, max * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    currentAlive = 0;
}

void ParticlesPool::Pour(glm::vec3 pos, int particlesPerFrame, float deltaTime) {
    int spawnParticles = particlesPerFrame * deltaTime;
    int toSpawn = static_cast<int>(spawnParticles);

    for (int i = 0; i < toSpawn; i++) {
        float vx = ((rand() % 100) / 100.f - 0.5f) * 0.2f;
        float vz = ((rand() % 100) / 100.f - 0.5f) * 0.2f;
        float speed = -1.5f * (rand() % 50) / 100.f;
        Spawn(pos, {vx, speed, vz});
    }
}

void ParticlesPool::Spawn(glm::vec3 position, glm::vec3 velocity) {
    if (currentAlive >= maxSize) {
        return;
    }
    Particles& p = particlesPool->at(currentAlive);
    p.setPos(position);
    p.setVelocity(velocity);
    p.alive = true;
    currentAlive++;
}

// TODO want it to be inforce not an out force
void ParticlesPool::Elipse(glm::vec3 pos, float deltaTime) {
    static float accumulativeTime = 0.0f;
    accumulativeTime += deltaTime;
    float angle = accumulativeTime * 10.0f;

    float xpos = pos.x + 0.05f * cos(angle);
    float ypos = pos.y + 0.03f * sin(angle);

    glm::vec3 velocity = glm::normalize(glm::vec3(cos(angle), sin(angle), 0.0f)) * 2.0f;
    Spawn({xpos, ypos, pos.z}, velocity);
}

void ParticlesPool::Update(float deltaTime) {
    int i = 0;
    while (i < currentAlive) {
        Particles& p = particlesPool->at(i);
        p.Update(deltaTime);
        if (p.alive == false) {
            std::swap(p, particlesPool->at(currentAlive - 1));
            currentAlive--;
        } else {
            i++;
        }
    }
}

void ParticlesPool::Render() {
    if (currentAlive == 0) {
        return;
    }

    std::vector<glm::vec3> positions;
    positions.reserve(currentAlive);

    for (int i = 0; i < currentAlive; i++) {
        positions.push_back(particlesPool->at(i).getPosition());
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_DYNAMIC_DRAW
    );

    glBindVertexArray(VAO);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glDrawArrays(GL_POINTS, 0, currentAlive);
    glBindVertexArray(0);
}
