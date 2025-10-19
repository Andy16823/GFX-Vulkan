#version 450

layout(location = 0) in vec3 pos;
layout(location = 0) out vec3 fragDir;

layout(set = 0, binding = 0) uniform UboViewProjection {
    mat4 projection;
    mat4 view;
} ubo;

void main() {
    // Entferne Translation aus der View-Matrix
    mat4 viewNoTranslation = mat4(mat3(ubo.view));

    // Transformiere die Position
    vec4 clipPos = ubo.projection * viewNoTranslation * vec4(pos, 1.0);

    // Spezialfall für Skybox: fixiere Tiefe auf W (hinter allen Objekten)
    gl_Position = clipPos.xyww;

    // Richtung zum Samplen der Cubemap
    fragDir = pos;
}
