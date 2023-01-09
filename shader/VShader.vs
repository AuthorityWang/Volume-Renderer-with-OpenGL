#version 400

layout (location = 0) in vec3 VerPos;
layout (location = 1) in vec3 VerClr;

out vec3 EntryPoint;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    EntryPoint = VerClr;
    gl_Position = proj * view * model * vec4(VerPos, 1.0);
}