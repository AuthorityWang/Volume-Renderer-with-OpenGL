#version 330 core

in vec3 EntryPoint;

layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(EntryPoint, 1.0);
    // FragColor = vec4(exitPoint, 1.0);
}