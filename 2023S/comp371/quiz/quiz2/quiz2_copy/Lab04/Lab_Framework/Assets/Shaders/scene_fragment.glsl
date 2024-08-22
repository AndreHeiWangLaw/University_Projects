#version 330 core

const float PI = 3.1415926535897932384626433832795;

uniform vec3 light_color;
uniform vec3 light_position; // Use the light_position output from the vertex shader
// Remove uniform vec3 light_direction;

uniform vec3 object_color;

const float shading_ambient_strength    = 0.1;
const float shading_diffuse_strength    = 0.6;
const float shading_specular_strength   = 0.3;

uniform float light_cutoff_outer;
uniform float light_cutoff_inner;
uniform float light_near_plane;
uniform float light_far_plane;

uniform vec3 view_position;

uniform sampler2D shadow_map;

in vec3 fragment_position;
in vec4 fragment_position_light_space;
in vec3 fragment_normal;
in vec3 light_position; // Add an input variable for light_position

in vec4 gl_FragCoord;

out vec4 result;

vec3 ambient_color(vec3 light_color_arg) {
    return shading_ambient_strength * light_color_arg;
}

vec3 diffuse_color(vec3 light_color_arg) { // Remove vec3 light_position_arg from the function signature
    vec3 light_direction = normalize(light_position - fragment_position);
    return shading_diffuse_strength * light_color_arg * max(dot(normalize(fragment_normal), light_direction), 0.0f);
}

vec3 specular_color(vec3 light_color_arg) { // Remove vec3 light_position_arg from the function signature
    vec3 light_direction = normalize(light_position - fragment_position);
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflect_light_direction = reflect(-light_direction, normalize(fragment_normal));
    return shading_specular_strength * light_color_arg * pow(max(dot(reflect_light_direction, view_direction), 0.0f), 32);
}

float shadow_scalar() {
    // No changes here since the shadow mapping technique remains the same
    // ...
}

void main()
{
    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);

    float scalar = shadow_scalar(); // Remove * spotlight_scalar()
    ambient = ambient_color(light_color);
    diffuse = scalar * diffuse_color(light_color);
    specular = scalar * specular_color(light_color);

    vec3 color = (specular + diffuse + ambient) * object_color;

    result = vec4(color, 1.0f);
}
