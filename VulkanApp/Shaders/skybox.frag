#version 450
layout(location = 0) in vec3 fragDir;
layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = vec4(abs(normalize(fragDir)), 1.0);
}
