//
// Created by teo on 29. 8. 2026.
//

#ifndef MAZE_LIGHT_H
#define MAZE_LIGHT_H

#pragma once

#define MAX_POINT_LIGHTS 128
#include "Grid.h"
#include <vector>
class Light {
public:

    static std::vector<Light> generatePointLight( Grid &grid,float minSpacing);

    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
    glm::vec3 color;
};
#endif  // MAZE_LIGHT_H
