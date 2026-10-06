//
// Created by teo on 22. 9. 2026.
//

#include "BoundingShapes.h"

#include <list>



BoundingShapes::BoundingShapes(glm::vec3 startingPoint,Shape shape , float height, float spread, float radius)
                                                            :  height(height), spread(spread), radius(radius) {

    switch  (shape) {
        case Cone:
            createCone(startingPoint);
            break;
        case Sphere:
            break;
        case Cube:
            break;
    }

}

bool BoundingShapes::intersect(BoundingBox& other){
//TODO implement SAT Separate Axis Theorem
    return true;
}

Interval BoundingShapes::getInterval(std::vector<glm::vec3>& points, int axis) {
    float min = 0.0f;
    float max = 0.0f;
    for (auto point : points) {
        if (min == max) {
            max = point[axis];
        }else {
            if (min >= point[axis]) {
                min = point[axis];
            }
            if (max  <= point[axis]) {
                max = point[axis];
            }
        }
    }
    return  Interval(min,max);
}


int BoundingShapes::createCone(glm::vec3 startingPoint) {

    if (spread <= 0.0f || radius <= 0.0f ) {
        return -1;
    }

    float halfSpread = spread / 2.0f;
    points.emplace_back(startingPoint.x - halfSpread, startingPoint.y, startingPoint.z + radius);
    points.emplace_back(startingPoint.x + halfSpread, startingPoint.y, startingPoint.z + radius);
    if (height > 0.0f) {
        points.emplace_back(startingPoint.x , startingPoint.y+ height, startingPoint.z);
        points.emplace_back(startingPoint.x - halfSpread, startingPoint.y + height, startingPoint.z + radius);
        points.emplace_back(startingPoint.x + halfSpread, startingPoint.y + height, startingPoint.z + radius);
    }
    return 0;
}
