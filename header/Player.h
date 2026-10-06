//
// Created by teo on 16. 9. 2026.
//

#pragma once
#ifndef MAZE_PLAYER_H
#define MAZE_PLAYER_H
#include "Camera.h"
#include "Spell.h"
#include "glm/vec3.hpp"

class Player
{
public:
    Player(const Camera& camera,glm::vec3 pos,int health , int stamina):camera(camera), HP(health), Stamina(stamina) , Position(pos) {};
    Player(const Camera& camera, float x ,float y ,float z, int health, int stamina):camera(camera), Position(x,y,z), HP(health), Stamina(stamina) {};
    int getHP() const{return HP;};
    int getStamina() const {return Stamina;};
    void setHP(int HP){this->HP = HP;};
    void setStamina(int Stamina){this->Stamina = Stamina;};
    void move(CameraMovement dir);
private:
    glm::vec3 Position;
    Camera camera;
    int HP = 100;
    int Stamina = 100;
    int speed = 10;
    std::vector<Spell> spells;

};


#endif //MAZE_PLAYER_H
