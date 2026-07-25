#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 2) in vec2 TexPos;

out vec2 textPos;

void main(){

    gl_Position = vec4(Pos,1.0);
    textPos = TexPos;

}