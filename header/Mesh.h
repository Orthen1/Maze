//
// Created by teo on 11. 8. 2026.
//

#ifndef MAZE_MESH_H
#define MAZE_MESH_H
#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"
//clang-format on


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int ID;
    std::string type;
};



class Mesh {
   public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indeces;
    std::vector<Texture> textures;
    Mesh(
        std::vector<Vertex> vertecies, std::vector<Texture> textures ,
        std::vector<unsigned int> indeces
    );
    void Draw(Shader& shader, GLenum Type);

   private:

    unsigned int VAO, VBO, EBO;

    void setUpMesh();
};

#endif  // MAZE_MESH_H
