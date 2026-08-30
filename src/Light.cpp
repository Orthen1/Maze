//
// Created by teo on 29. 8. 2026.
//

#include "Light.h"

std::vector<Light> Light::generatePointLight(Grid& grid, float minSpacing) {
    std::vector<Light> lights;
    int numOfLights = 0;
    for (auto& cell : grid.getCells()) {
        std::mt19937 random(std::random_device{}());
        std::uniform_real_distribution<float> spawnChance(0.0f, 1.0f);
        if (numOfLights == MAX_POINT_LIGHTS) {
            break;
        }
        if ((spawnChance(random) >= 0.15f) && cell->isWall == false) {
            glm::vec3 position(cell->worldPos.x, cell->worldPos.y, cell->worldPos.z);
            bool tooClose = false;
            for (auto& exist : lights) {
                // Check if light randomized position is'nt too close to other light sources
                if (glm::distance(exist.position, position) < minSpacing) {
                    tooClose = true;
                    break;
                }
            }
            if (!tooClose) {
                glm::vec3 color(0.95f, 0.48f, 0.01f);

                float constant = 1.0f;
                float linear = 0.09f;
                float quadratic = 0.032f;
                lights.insert(lights.end(), {position, constant, linear, quadratic, color});

                numOfLights++;
            }
        }
    }
    return lights;
}
