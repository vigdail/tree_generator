#version 460 core

#extension GL_ARB_shading_language_include: require

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

layout(location = 0) out vec3 v_normal;

#include "/scene.glsl"

void main() {
    v_normal = normal;
    gl_Position = view_proj * vec4(pos, 1.0);
}
