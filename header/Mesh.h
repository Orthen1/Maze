//
// Created by teo on 11. 8. 2026.
//

#ifndef MAZE_MESH_H
#define MAZE_MESH_H
#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>

class Mesh {
   public:
    Mesh(float* vertex, int size);
    unsigned int GetVBO() { return VBO; };

   private:
    std::vector<float> vertecies;
    unsigned int VBO;
};

#endif  // MAZE_MESH_H
