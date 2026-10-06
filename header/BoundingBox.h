//
// Created by teo on 29. 8. 2026.
//

#ifndef MAZE_BOUNDINGBOX_H
#define MAZE_BOUNDINGBOX_H
#include <vector>

#include "Mesh.h"
#include "glm/vec3.hpp"

/// An axis-aligned bounding box (AABB), used for collision detection.
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
    /// Returns true if this box overlaps `other` on all three axes (AABB overlap test).
    bool intersect(const BoundingBox& other) const {
        return minX <= other.maxX && maxX >= other.minX && minY <= other.maxY &&
               maxY >= other.minY && minZ <= other.maxZ && maxZ >= other.minZ;
    }
    /// Converts bounding box to Vertecies for debug function
   std::vector<Vertex> boxToVertex() {
        return {
            Vertex({minX, minY, minZ}),
            Vertex({maxX, minY, minZ}),
            Vertex({maxX, minY, maxZ}),
            Vertex({minX, minY, maxZ}),
            Vertex({minX, maxY, minZ}),
            Vertex({maxX, maxY, minZ}),
            Vertex({maxX, maxY, maxZ}),
            Vertex({minX, maxY, maxZ})
        };
    };
};
#endif  // MAZE_BOUNDINGBOX_H
