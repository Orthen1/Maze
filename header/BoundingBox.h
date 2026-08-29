//
// Created by teo on 29. 8. 2026.
//

#ifndef MAZE_BOUNDINGBOX_H
#define MAZE_BOUNDINGBOX_H
#include <vector>

struct BoundingBox {
    float minX, minY, minZ, maxX, maxY, maxZ;
    BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) {
        this->minX = minX;
        this->minY = minY;
        this->minZ = minZ;
        this->maxX = maxX;
        this->maxY = maxY;
        this->maxZ = maxZ;
    }

    bool intersect(BoundingBox& other) const {
        return minX <= other.maxX && maxX >= other.minX && minY <= other.maxY &&
               maxY >= other.minY && minZ <= other.maxZ && maxZ >= other.minZ;
    }

    std::vector<float> boxToVertex() {
        return {minX, minY, minZ, maxX, minY, minZ, maxX, minY, maxZ, minX, minY, maxZ,

                minX, maxY, minZ, maxX, maxY, minZ, maxX, maxY, maxZ, minX, maxY, maxZ};
    };
};
#endif  // MAZE_BOUNDINGBOX_H
