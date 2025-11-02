#version 450

//////////////////////////////////////////////////////////////////
// Unlit 3D Vertex Shader with Instancing
//////////////////////////////////////////////////////////////////

// Vertex Input
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vcolor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;

// Camera Uniforms Set 0
layout(set = 0, binding = 0) uniform UboViewProjection {
    mat4 projection;
    mat4 view;
    vec3 cameraPos; // unused in this shader
} uboViewProjection;

// Instance Data Set 1
struct InstanceData {
    mat4 model;
    vec4 extras;  // x=visible, y=roughness, z=metallic, w=custom
};

layout(std430, set = 1, binding = 0) readonly buffer InstanceBuffer {
    InstanceData instances[];
};

// Passing to Fragment Shader
layout(location = 0) out vec3 color;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec4 fragExtras;

// Main Function
void main() {
    // Get current instance data
    InstanceData instance = instances[gl_InstanceIndex];

    // Transform instance position
    mat4 model = instance.model;
    gl_Position = uboViewProjection.projection * uboViewProjection.view * model * vec4(pos, 1.0);


    // Transform normal (für Lighting)
    mat3 normalMatrix = mat3(transpose(inverse(instance.model)));
    fragNormal = normalize(normalMatrix * normal);

    // Pass-through
    color = vcolor;
    fragTexCoord = texCoord;
    fragExtras = instance.extras;
}