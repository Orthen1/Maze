//
// Created by teo on 22. 9. 2026.
//
#pragma once
#ifndef MAZE_BOUNDINGSHAPES_H
#define MAZE_BOUNDINGSHAPES_H
#include "BoundingBox.h"
#include "glm/vec3.hpp"
#include "enums.h"


struct Interval {
    float min;
    float max;
};


class BoundingShapes {

public:
    BoundingShapes(glm::vec3 startingPoint,Shape shape, float height, float spread, float radius);
    bool intersect(BoundingBox& other);

private:
    float height;
    float spread;
    float radius;
    std::vector<glm::vec3> points;

    Interval getInterval(std::vector<glm::vec3>& points, int  axis );
    int createCone(glm::vec3 startingPoint);


};








#endif //MAZE_BOUNDINGSHAPES_H
