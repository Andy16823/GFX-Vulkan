#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vcolor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;

layout(set = 0, binding = 0) uniform UboViewProjection {
    mat4 projection;
    mat4 view;
    vec3 cameraPos;
} uboViewProjection;

layout(push_constant) uniform PushConstants {
    mat4 model;
} pushConstants;

layout(location = 0) out vec3 color;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragWorldPos;

void main() {
    // Calculate world position
    vec4 worldPos = pushConstants.model * vec4(pos, 1.0);
    fragWorldPos = worldPos.xyz;

    // Transform position
    gl_Position = uboViewProjection.projection * uboViewProjection.view * worldPos;

    // Transform normal (für Lighting)
    mat3 normalMatrix = mat3(transpose(inverse(pushConstants.model)));
    fragNormal = normalize(normalMatrix * normal);

    // Pass-through
    color = vcolor;
    fragTexCoord = texCoord;
}