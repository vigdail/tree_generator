#version 460 core

#extension GL_ARB_shading_language_include: require

layout(location = 0) in vec3 pos;

#include "/scene.glsl"

void main() {
    gl_Position = view_proj * vec4(pos, 1.0);
}
