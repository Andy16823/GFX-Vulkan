#version 450
layout(location = 0) in vec3 fragDir;
layout(location = 0) out vec4 fragColor;

layout(set = 1, binding = 0) uniform samplerCube skybox;

void main() {
    fragColor = texture(skybox, fragDir);
}
