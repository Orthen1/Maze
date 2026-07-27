//
// Created by teo on 27. 7. 2026.
//

#ifndef MAZE_WALL_H
#define MAZE_WALL_H
#include <glm/vec3.hpp>



class Wall {


public:
    glm::vec3 position;

    static constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, 0.0f,  0.0f, 0.0f,

        0.5f,  -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f,  -0.5f,0.5f, 0.0f, 0.0f,
        0.5f,  0.5f, 0.5f,  0.0f, 0.0f,
        0.5f,  0.5f, 0.0f,  1.0f, 0.0f,

        -0.5f,  0.5f,0.5f, 0.0f, 1.0f,
        0.5f,  0.5f, 0.5f,  0.0f, 1.0f,

        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f, 0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, 0.5f,  0.0f, 0.0f,
    };

    static  constexpr int indeces[] = {
        //BACk
        0,1,2,
        1,2,3,

        //DOWN
        0,1,4,
        0,4,5,

        //RIGHT
        6,1,7,
        6,4,1,

        //LEFT
        0,2,5,
        2,5,8,

        //UP
        2,3,9,
        2,6,8,

        //FRONT
        10,11,12,
        11,12,13,



    };

};



#endif //MAZE_WALL_H
