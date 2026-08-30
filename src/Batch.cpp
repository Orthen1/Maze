//
// Created by teo on 29. 8. 2026.
//

#include "Batch.h"
/**
 * @brief Builds a single batched mesh containing every visible wall, by
 *        instancing `cubeVertices`/`indeces` (one cube) at each wall's position.
 */
BatchGeometry createWallBatch(
    Grid& grid, const std::vector<float>& cubeVertices, const std::vector<unsigned int>& indeces
) {
    std::vector<float> mazeWallVerteces;
    std::vector<unsigned int> mazeWallIndeces;

    unsigned int wallCounter = 0;

    for (auto& wall : grid.getWalls()) {
        if (!wall->visible) {
            continue;
        }
        unsigned int wallBase = wallCounter * 24;
        for (int i = 0; i < cubeVertices.size(); i += 8) {
            mazeWallVerteces.push_back(cubeVertices[i + 0] + wall->position.x);
            mazeWallVerteces.push_back(cubeVertices[i + 1] + wall->position.y);
            mazeWallVerteces.push_back(cubeVertices[i + 2] + wall->position.z);
            mazeWallVerteces.push_back(cubeVertices[i + 3]);
            mazeWallVerteces.push_back(cubeVertices[i + 4]);
            mazeWallVerteces.push_back(cubeVertices[i + 5]);
            mazeWallVerteces.push_back(cubeVertices[i + 6]);
            mazeWallVerteces.push_back(cubeVertices[i + 7]);
        }
        for (auto& index : indeces) {
            mazeWallIndeces.push_back(wallBase + index);
        }
        wallCounter++;
    }

    return {mazeWallVerteces, mazeWallIndeces};
}
/**
 * @brief Builds a batched mesh of horizontal quads (one per open/exit cell),
 *        used for both the floor and ceiling.
 * @param grid     The maze grid to read cells from.
 * @param exitPos  World-space position of the exit cell — included even
 *                  though it's technically a wall cell, so the exit opening
 *                  still gets floor/ceiling geometry.
 * @param yOffset  Vertical offset from each cell's worldPos (e.g. -0.5 for
 *                  floor, +0.5x for ceiling).
 * @param normalY  Y component of the quad's normal (1.0 facing up for
 *                  floor, -1.0 facing down for ceiling).
 */
BatchGeometry createHorizontalQuadBatch(
    Grid& grid, const glm::vec3 exitPos, float yOffset, float normalY
) {
    std::vector<float> verts;
    std::vector<unsigned int> idx;
    unsigned int counter = 0;

    for (auto& cell : grid.getCells()) {
        if (cell->isWall && (cell->worldPos != exitPos)) {
            continue;
        }
        glm::vec3 p = cell->worldPos;
        float quad[] = {
            p.x - 0.5f, p.y + yOffset, p.z - 0.5f, 0.0f, normalY, 0.0f, 0.0f, 0.0f,
            p.x + 0.5f, p.y + yOffset, p.z - 0.5f, 0.0f, normalY, 0.0f, 1.0f, 0.0f,
            p.x + 0.5f, p.y + yOffset, p.z + 0.5f, 0.0f, normalY, 0.0f, 1.0f, 1.0f,
            p.x - 0.5f, p.y + yOffset, p.z + 0.5f, 0.0f, normalY, 0.0f, 0.0f, 1.0f,
        };
        verts.insert(verts.end(), std::begin(quad), std::end(quad));

        unsigned int index = counter * 4;
        idx.insert(idx.end(), {index + 0, index + 1, index + 2, index + 0, index + 2, index + 3});
        counter++;
    }
    return {verts, idx};
}
// Batched floor quads: one per open cell (plus the exit cell), facing up.
BatchGeometry createFloorBatch(Grid& grid, const glm::vec3 exitPos) {
    return createHorizontalQuadBatch(grid, exitPos, -0.5f, 1.0f);
}
// Batched ceiling quads: one per open cell (plus the exit cell), facing down.
BatchGeometry createCeilingBatch(Grid& grid, const glm::vec3 exitPos) {
    return createHorizontalQuadBatch(grid, exitPos, 0.5f, -1.0f);
}
/**
 * @brief Builds wireframe box geometry visualizing every wall's bounding
 *        box, for collision debugging.
 */
DebugGeometry buildBoundingBoxDebugGeometry(Grid& grid) {
    std::vector<float> unitBoxVerts;
    std::vector<unsigned int> boxLineIndices;

    unsigned int counter = 0;
    for (auto& wall : grid.getWalls()) {
        // Each box contributes 8 vertices (its corners); offset indices
        // per-wall so they reference this wall's own corners.
        unsigned int base = counter * 8;

        std::vector<float> boxVerts = wall->box.boxToVertex();
        unitBoxVerts.insert(unitBoxVerts.end(), boxVerts.begin(), boxVerts.end());
        boxLineIndices.insert(
            boxLineIndices.end(),
            {

                base + 0, base + 1, base + 1, base + 2, base + 2, base + 3, base + 3, base + 0,

                base + 4, base + 5, base + 5, base + 6, base + 6, base + 7, base + 7, base + 4,

                base + 0, base + 4, base + 1, base + 5, base + 2, base + 6, base + 3, base + 7,

            }
        );

        counter++;
    }
    return {unitBoxVerts, boxLineIndices};
}
/*
 * @brief Builds wireframe outline geometry for every floor (non-wall) tile,
 *        for debugging grid layout/cell boundaries.
 */

DebugGeometry buildTileDebugGeometry(Grid& grid) {
    DebugGeometry result;
    unsigned int counter = 0;
    for (auto& cell : grid.getCells()) {
        if (cell->isWall) {
            continue;  // only floor tiles
        }

        std::vector<float> tileVerts = {
            cell->worldPos.x - 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z - 0.5f,
            cell->worldPos.x + 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z - 0.5f,
            cell->worldPos.x + 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z + 0.5f,
            cell->worldPos.x - 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z + 0.5f,
        };
        result.vertcies.insert(result.vertcies.end(), tileVerts.begin(), tileVerts.end());

        unsigned int base = counter * 4;
        result.indeces.insert(
            result.indeces.end(), {
                                      base + 0,
                                      base + 1,
                                      base + 1,
                                      base + 2,
                                      base + 2,
                                      base + 3,
                                      base + 3,
                                      base + 0,
                                  }
        );
        counter++;
    }
    return result;
}
