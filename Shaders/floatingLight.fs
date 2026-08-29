#version 330 core

out vec4 fragColor;

void main() {
    vec3 Color = vec3(0.95, 0.48, 0.01);
    fragColor = vec4(Color, 1.0);
}