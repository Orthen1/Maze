//
// Created by teo on 16. 9. 2026.
//

#ifndef MAZE_SPELL_H
#define MAZE_SPELL_H
#include "BoundingBox.h"
#include "glm/vec3.hpp"
#include "enums.h"


class Spell
{
public:
    Spell(int DMG, int speed, Shape shape, float radius, glm::vec3 pos): damage(DMG), speed(speed), radius(radius), bb(createBoundingBox(shape)){};
    void setDMG(int DMG){damage = DMG;};
    void setSpeed(int speed){this->speed = speed;};
    int getDMG(){return damage;};
    int getSpeed(){return speed;};

private:
    BoundingBox createBoundingBox(Shape shape) const;

    glm::vec3 Pos;
    BoundingBox bb;
    float radius;
    int damage;
    int speed;
    int VAO;
    int VBO;


};


#endif //MAZE_SPELL_H
