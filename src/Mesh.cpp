//
// Created by teo on 11. 8. 2026.
//

#include "../header/Mesh.h"



template <size_t N>
 Mesh::Mesh(float (&vertex)[N]){

     this->vertecies = vertex;
     glGenBuffers(1,&VBO);
     glBindBuffer(GL_ARRAY_BUFFER,VBO);
     glBufferData(GL_ARRAY_BUFFER,N*sizeof(float),vertex,GL_STATIC_DRAW);
     glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3* sizeof(vertex),(void*)(sizeof(float)*0));
     glEnableVertexAttribArray(0);


}