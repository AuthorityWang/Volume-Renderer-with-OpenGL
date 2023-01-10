#version 400
in vec3 EntryPoint;

uniform sampler2D ExitPoints;
uniform sampler3D VolumeTex;
uniform sampler1D TransferFunc;
uniform float     StepSize;
uniform vec2      ScreenSize;
layout (location = 0) out vec4 FragColor;

void main()
{
    vec3 exitPoint = texture(ExitPoints, gl_FragCoord.st/ScreenSize).xyz;

	vec3 ray = exitPoint - EntryPoint;

    float ray_length = length(ray);
    vec3 step_vector = StepSize * (ray / ray_length);

    vec3 position = EntryPoint;
    vec4 color = vec4(0.0);

    float acum_length = 0.0;
    while (acum_length <= ray_length && color.a < 1.0) {
        float intensity = texture(VolumeTex, position).r;
        vec4 c = texture(TransferFunc, intensity);
        // ray casting cal rgba
        color.rgb = c.a * c.rgb + (1 - c.a) * color.a * color.rgb;
        color.a = c.a + (1 - c.a) * color.a;
        acum_length += StepSize;
        position += step_vector;
    }

    color.xyz = color.a * color.rgb + (1 - color.a) * vec3(1.0);
    color.w = 1.0;
    FragColor = color;
}
