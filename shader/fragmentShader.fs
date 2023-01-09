#version 330 core
out vec4 FragColor;

uniform mat4 ViewMatrix;
uniform float focal_length;
uniform float aspect_ratio;
uniform vec2 viewport_size;
uniform vec3 ray_origin;
uniform vec3 upp_bnd;
uniform vec3 low_bnd;
uniform vec3 background_colour;
uniform float step_size;
uniform sampler3D volume;
uniform sampler1D TranFunc;
uniform float gamma;

// // Ray struct
struct Ray {
    vec3 origin;
    vec3 direction;
};

// // bounding box for the whole raw data
struct AABB {
    vec3 low_bnd;
    vec3 upp_bnd;
};

void intersect(Ray ray, AABB bbox, out float t_0, out float t_1) {
    vec3 normalizedd = 1.0 / ray.direction;
    vec3 upp = normalizedd * (bbox.upp_bnd - ray.origin);
    vec3 low = normalizedd * (bbox.low_bnd - ray.origin);
    vec3 tmin = min(upp, low);
    vec2 t = max(tmin.xx, tmin.yz);
    t_0 = max(0.0, max(t.x, t.y));
    vec3 tmax = max(upp, low);
    t = min(tmax.xx, tmax.yz);
    t_1 = min(t.x, t.y);
};

void main() {
    vec3 ray_direction;
    ray_direction.xy = 2.0 * gl_FragCoord.xy / viewport_size - 1.0;
    ray_direction.x *= aspect_ratio;
    ray_direction.z = -focal_length;
    ray_direction = (vec4(ray_direction, 0) * ViewMatrix).xyz;

    float t_0, t_1;
    Ray m_ray = Ray(ray_origin, ray_direction);
    AABB bbox = AABB(upp_bnd, low_bnd);
    intersect(m_ray, bbox, t_0, t_1);

    vec3 ray_start = (ray_origin + ray_direction * t_0 - low_bnd) / (upp_bnd - low_bnd);
    vec3 ray_stop = (ray_origin + ray_direction * t_1 - upp_bnd) / (upp_bnd - low_bnd);

    vec3 ray = ray_stop - ray_start;
    float ray_length = length(ray);
    vec3 step_vector = step_size * (ray / ray_length);

    vec3 position = ray_start;
    vec4 color = vec4(0.0);

    float acum_length = 0.0;
    while (acum_length <= ray_length && color.a < 1.0) {
        float intensity = texture(volume, position).r;
        vec4 c = texture(TranFunc, intensity);
        // ray casting cal rgba
        color.rgb = c.a * c.rgb + (1 - c.a) * color.a * color.rgb;
        color.a = c.a + (1 - c.a) * color.a;

        acum_length += step_size;
        position += step_vector;
    }

    color.rgb = color.a * color.rgb + (1 - color.a) * background_colour;
    color.a = 1.0;

    FragColor = vec4(0.0, 1.0, 1.0, 1.0);
    // FragColor = color;
    // if (color.x > 0.0) {
    //     FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    // } else {
    //     FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    // }
    // FragColor = vec4()
}