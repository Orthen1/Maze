//
// Created by teo on 29. 8. 2026.
//

#include "Batch.h"

BatchGeometry createWallBatch(Grid &grid,const std::vector<float>& cubeVertices,  const std::vector<unsigned int> &indeces ) {
    std::vector<float> mazeWallVerteces;
    std::vector<unsigned int> mazeWallIndeces;

    unsigned int wallCounter = 0;


    for (auto& wall: grid.getWalls()) {
        if (!wall->visible) {
            continue;
        }
        unsigned int wallBase = wallCounter * 24;
        for (int i = 0 ; i < cubeVertices.size(); i+= 8) {
            mazeWallVerteces.push_back(cubeVertices[i+0]+wall->position.x);
            mazeWallVerteces.push_back(cubeVertices[i+1]+wall->position.y);
            mazeWallVerteces.push_back(cubeVertices[i+2]+wall->position.z);
            mazeWallVerteces.push_back(cubeVertices[i+3]);
            mazeWallVerteces.push_back(cubeVertices[i+4]);
            mazeWallVerteces.push_back(cubeVertices[i+5]);
            mazeWallVerteces.push_back(cubeVertices[i+6]);
            mazeWallVerteces.push_back(cubeVertices[i+7]);
        }
        for (auto &index : indeces) {
            mazeWallIndeces.push_back(wallBase+index);
        }
        wallCounter++;
    }


    return {mazeWallVerteces,mazeWallIndeces};
}
BatchGeometry createFloorBatch(Grid &grid, const glm::vec3 exitPos) {
    std::vector<float> floorVerts;
    std::vector<unsigned int> floorIdx;
    unsigned int counter = 0;
    for (auto &cell: grid.getCells()) {

        if (cell->isWall && (cell->worldPos != exitPos)) {

            continue;
        }
        glm::vec3 p = cell->worldPos;
        float fVerts[] = {
            p.x - 0.5f, p.y - 0.51f, p.z - 0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
            p.x + 0.5f, p.y - 0.51f, p.z - 0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            p.x + 0.5f, p.y - 0.51f, p.z + 0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
            p.x - 0.5f, p.y - 0.51f, p.z + 0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        };

        floorVerts.insert(floorVerts.end(), std::begin(fVerts), std::end(fVerts));
        unsigned int index= counter * 4;
        floorIdx.insert(floorIdx.end(), {index+0, index+1, index+2, index+0, index+2, index+3});

    counter++;
    }
    return {floorVerts,floorIdx};
}
BatchGeometry createCeilingBatch(Grid &grid, const glm::vec3 exitPos) {
    std::vector<float>  ceilingVerts;
    std::vector<unsigned int>  ceilingIdx;
    unsigned int counter = 0;
    for (auto &cell: grid.getCells()) {

        if (cell->isWall && (cell->worldPos != exitPos)) {
            continue;
        }
        glm::vec3 p = cell->worldPos;
        float cVerts[] = {
            p.x - 0.5f, p.y + 0.51f, p.z - 0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
            p.x + 0.5f, p.y + 0.51f, p.z - 0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
            p.x + 0.5f, p.y + 0.51f, p.z + 0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
            p.x - 0.5f, p.y + 0.51f, p.z + 0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
        };
        ceilingVerts.insert(ceilingVerts.end(), std::begin(cVerts), std::end(cVerts));
        unsigned int index= counter * 4;
        ceilingIdx.insert(ceilingIdx.end(), {index+0, index+1, index+2, index+0, index+2, index+3});
        counter++;
    }
    return {ceilingVerts,ceilingIdx};
}


DebugGeometry buildBoundingBoxDebugGeometry( Grid& grid) {
    std::vector<float> unitBoxVerts;
    std::vector<unsigned int> boxLineIndices;

    unsigned int counter = 0;
    for (auto& wall : grid.getWalls()) {
        unsigned int base = counter * 8;

        std::vector<float> boxVerts = wall->box.boxToVertex();
        unitBoxVerts.insert(unitBoxVerts.end(), boxVerts.begin(), boxVerts.end());
        boxLineIndices.insert(
            boxLineIndices.end(), {

                                      base + 0, base + 1, base + 1, base + 2, base + 2, base + 3, base + 3, base + 0,

                                      base + 4, base + 5, base + 5, base + 6, base + 6, base + 7, base + 7, base + 4,

                                      base + 0, base + 4, base + 1, base + 5, base + 2, base + 6, base + 3, base + 7,

                                  }
        );

        counter++;
    }
    return {unitBoxVerts,boxLineIndices};
}

DebugGeometry buildTileDebugGeometry(Grid& grid) {
    DebugGeometry result;
    unsigned int counter = 0;
    for (auto& cell : grid.getCells()) {
        if (cell->isWall) continue;  // only floor tiles

        std::vector<float> tileVerts = {
            cell->worldPos.x - 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z - 0.5f,
            cell->worldPos.x + 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z - 0.5f,
            cell->worldPos.x + 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z + 0.5f,
            cell->worldPos.x - 0.5f, cell->worldPos.y - 0.5f, cell->worldPos.z + 0.5f,
        };
        result.vertcies.insert(result.vertcies.end(), tileVerts.begin(), tileVerts.end());

        unsigned int base = counter * 4;
        result.indeces.insert(result.indeces.end(), {
            base+0, base+1, base+1, base+2, base+2, base+3, base+3, base+0,
        });
        counter++;
    }
    return result;
}
