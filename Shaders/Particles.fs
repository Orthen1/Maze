#version 330 core

out vec4 fragColor;

void main() {
    vec3 Color = vec3(1.0, 0.8, 0.0);
    fragColor = vec4(Color, 1.0);
}