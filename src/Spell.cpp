//
// Created by teo on 16. 9. 2026.
//

#include "../header/Spell.h"



BoundingBox Spell::createBoundingBox(Shape shape) const {
    switch (shape) {
        case Cone:
            // TODO change into polyhedron for bounds
            return  BoundingBox(Pos.x,Pos.y,Pos.z,Pos.x + radius, Pos.y, Pos.z + radius);
    }
}