//
// Created by teo on 11. 8. 2026.
//

#ifndef MAZE_MESH_H
#define MAZE_MESH_H
#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
//clang-format on
struct vertAttribute {
    GLuint index;
    GLuint size;
};

class Mesh {
   public:
    Mesh(
        std::vector<float> vertex, int size, std::vector<vertAttribute> attributes,
        std::vector<uint> indeces = {}, int indexCount = 0
    );
    unsigned int getVAO() { return VAO; };

   private:
    std::vector<float> vertecies;

    unsigned int VAO, VBO, EBO;
};

#endif  // MAZE_MESH_H
