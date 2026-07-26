#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 2) in vec2 TexPos;


uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;


out vec2 textPos;

void main(){

    gl_Position =  projection * view * model *vec4(Pos,1.0);
    textPos = TexPos;

}