#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragUV;

void main() {
    // inPos is already in NDC for a fullscreen quad (-1..1)
    gl_Position = vec4(inPos, 1.0);
    fragColor = inColor;
    fragUV = inUV;
}