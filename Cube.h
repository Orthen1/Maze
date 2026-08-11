//
// Created by teo on 30. 7. 2026.
//

#ifndef MAZE_CUBE_H
#define MAZE_CUBE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Cube {
public:
    glm::vec3 position;
    glm::mat4 model;



    Cube(glm::vec3 pos);
    Cube(float x, float y, float z);
    glm::mat4 GetModel();
    glm::vec3 GetPosition();





};




#endif //MAZE_CUBE_H
