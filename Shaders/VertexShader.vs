#version 330 core

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Norm;
layout(location = 2) in vec2 UV;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 Normal;
out vec3 FragPos;
out vec2 textPos;

void main() {
    gl_Position = projection * view * model * vec4(Pos, 1.0);
    FragPos = vec3(view * model * vec4(Pos, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * Norm;
    textPos = UV;
}