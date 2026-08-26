//
// Created by teo on 11. 8. 2026.
//

#include "../header/Mesh.h"



 Mesh::Mesh(float *vertex, int size){

     this->vertecies.assign(vertex,vertex + size);
     glGenBuffers(1,&VBO);
     glBindBuffer(GL_ARRAY_BUFFER,VBO);
     glBufferData(GL_ARRAY_BUFFER,size*sizeof(float),vertecies.data(),GL_STATIC_DRAW);
     glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5* sizeof(float),(void*)(sizeof(float)*0));
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float), (void*)(sizeof(float)*3));

     glEnableVertexAttribArray(0);
     glEnableVertexAttribArray(1);

}
