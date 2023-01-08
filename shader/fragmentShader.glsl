#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 direction;
uniform vec3 viewPos;
uniform float cutOff;
uniform float outerCutOff;

in vec3 FragPos;
in vec3 Normal;

void main()
{
    vec3 result = vec3(0.0, 0.0, 0.0);
    FragColor = vec4(result, 1.0);
}