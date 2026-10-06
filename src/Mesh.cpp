//
// Created by teo on 11. 8. 2026.
//

#include "../header/Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indeces) {
    this->vertices = vertices;
    this->textures = textures;
    this->indeces = indeces;

    setUpMesh();
}


void Mesh::setUpMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indeces.size() * sizeof(unsigned int), &indeces[0],GL_STATIC_DRAW);

    //Vertex Position

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3,GL_FLOAT, GL_FALSE,sizeof(Vertex),(void*)0);

    //Vertex Normal

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT  ,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,Normal));

    //Vertex TexCoords

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,TexCoords));
}

void Mesh::Draw(Shader& shader, GLenum Type) {
    unsigned int diffuseN = 1;
    unsigned int specularN = 1 ;

    for (unsigned int i = 0; i < textures.size();i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse" ) {
            number =    std::to_string(diffuseN++);
        }else if (name == "texture_specular") {
            number = std::to_string(specularN++);
        }

        shader.setInt(("material."+name+number),i);
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);

    }

    glBindVertexArray(VAO);
    glDrawElements(Type,indeces.size(), GL_UNSIGNED_INT,0);
    glBindVertexArray(0);



}
