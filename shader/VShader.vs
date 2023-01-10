#version 400

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;  


out vec3 EntryPoint;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    EntryPoint = aCol;
    gl_Position = proj * view * model * vec4(aPos,1.0);
}