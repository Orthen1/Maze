//
// Created by teo on 11. 8. 2026.
//

#ifndef MAZE_MESH_H
#define MAZE_MESH_H
#pragma once
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
//clang-format on

/// Describes a single vertex attribute layout
struct vertAttribute {
    GLuint index; ///  Attribute location (matches `layout(location = index)
    GLuint size; /// Number of components for this attribute
};
/**
 * @brief A renderable mesh: owns vertex (and optional index) data and the
 *        OpenGL objects needed to draw it.
 */
class Mesh {
   public:
    /**
 * @brief Uploads vertex data to the GPU and configures vertex attributes.
 * @param vertex     Flat array of interleaved vertex data, laid out
 *                   according to `attributes`.
 * @param size       Total number of floats in `vertex` (used to compute
 *                   buffer size / vertex count alongside `attributes`).
 * @param attributes Layout of each vertex attribute (location + component
 *                   count) to configure via glVertexAttribPointer.
 * @param indeces    Optional index data, for indexed drawing (glDrawElements).
 *                   If empty, the mesh is drawn non-indexed.
 * @param indexCount Number of indices in `indeces`, if using indexed drawing.
 */
    Mesh(
        std::vector<float> vertex, int size, std::vector<vertAttribute> attributes,
        std::vector<unsigned int> indeces = {}, int indexCount = 0
    );
    unsigned int getVAO() { return VAO; };

   private:
    std::vector<float> vertecies;

    unsigned int VAO, VBO, EBO;
};

#endif  // MAZE_MESH_H
