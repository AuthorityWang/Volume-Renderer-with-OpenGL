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
    // flashlight
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 
    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-direction)); 
    float epsilon = (cutOff - outerCutOff);
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
        
    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
}