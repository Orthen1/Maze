//
// Created by teo on 11. 8. 2026.
//

#ifndef MAZE_MESH_H
#define MAZE_MESH_H
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>



class Mesh {

public:
    template <size_t N>
    Mesh(float (&vertex)[N]);
    unsigned int GetVBO() { return VBO; };

private:
    float *vertecies;
    unsigned int VBO;

};



#endif //MAZE_MESH_H
