//
// Created by teo on 11. 8. 2026.
//

#include "../header/Mesh.h"

Mesh::Mesh(
    std::vector<float> vertex, int size, const std::vector<vertAttribute> attributes,
    std::vector<uint> indeces, int indexCount
) {
    this->vertecies = vertex;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    GLuint stride = 0;
    for (auto& attribute : attributes) {
        stride += attribute.size;
    }

    GLuint offset = 0;
    for (auto& attribute : attributes) {
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertecies.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(
            attribute.index, attribute.size, GL_FLOAT, GL_FALSE, stride * sizeof(float),
            (void*)(sizeof(float) * offset)
        );
        glEnableVertexAttribArray(attribute.index);
        offset += attribute.size;
    }

    if (!indeces.empty()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint), indeces.data(), GL_STATIC_DRAW
        );
    }

    glBindVertexArray(0);

    glEnableVertexAttribArray(0);
}
