//
// Created by teo on 29. 8. 2026.
//

#ifndef MAZE_BATCH_H
#define MAZE_BATCH_H
#pragma once
#include <vector>

#include "Grid.h"

struct BatchGeometry {
    std::vector<float> vertcies;
    std::vector<unsigned int> indeces;
};

BatchGeometry createWallBatch(
    Grid& grid, const std::vector<float>& cubeVertecies, const std::vector<unsigned int>& indeces
);
BatchGeometry createFloorBatch(Grid& grid, const glm::vec3 exitPos);
BatchGeometry createCeilingBatch(Grid& grid, const glm::vec3 exitPos);

struct DebugGeometry {
    std::vector<float> vertcies;
    std::vector<unsigned int> indeces;
};

DebugGeometry buildBoundingBoxDebugGeometry(Grid& grid);

#endif  // MAZE_BATCH_H
