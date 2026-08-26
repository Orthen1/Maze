#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 UV;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;


out vec2 textPos;

void main(){

    gl_Position =  projection * view * model *vec4(Pos,1.0);
    textPos = UV;
}