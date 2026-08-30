//
// Created by teo on 29. 8. 2026.
//

#ifndef MAZE_LIGHT_H
#define MAZE_LIGHT_H

#pragma once

#define MAX_POINT_LIGHTS 128
#include <vector>

#include "Grid.h"

/**
 * @brief A point light source: position, attenuation coefficients, and color.
 *
 * Attenuation follows the standard OpenGL point-light falloff formula:
 * attenuation = 1 / (constant + linear * d + quadratic * d^2), where d is
 * distance from the light.
 */
class Light {
   public:
    /**
     * @brief Generates a set of point lights placed throughout the maze,
     *        spaced at least `minSpacing` apart.
     * @param grid       The maze grid to place lights within.
     * @param minSpacing Minimum world-space distance allowed between lights.
     * @return A vector of generated Light instances.
     */
    static std::vector<Light> generatePointLight(Grid& grid, float minSpacing);

    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
    glm::vec3 color;
};
#endif  // MAZE_LIGHT_H
