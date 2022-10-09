#version 460 core

layout(location = 0) in vec3 v_normal;
layout(location = 0) out vec4 frag_color;

uniform vec3 color;
uniform vec3 light_dir = normalize(vec3(1.0, 1.0, 1.0));
uniform float light_intensity = 1.0;

void main() {
    float diffuse = max(dot(normalize(v_normal), light_dir), 0.0);
    float ambient = 0.3;
    frag_color = vec4(color * min((diffuse + ambient), 1.0) * light_intensity, 1.0);
}
