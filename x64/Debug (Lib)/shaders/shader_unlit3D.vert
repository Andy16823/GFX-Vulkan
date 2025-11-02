#version 450

//////////////////////////////////////////////////////////////////
// Unlit 3D Vertex Shader
//////////////////////////////////////////////////////////////////

// Vertex Input
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vcolor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;

// Camera Uniforms
layout(set = 0, binding = 0) uniform UboViewProjection {
    mat4 projection;
    mat4 view;
    vec3 cameraPos;
} uboViewProjection;

// Model Matrix push constants
layout(push_constant) uniform PushConstants {
    mat4 model;
} pushConstants;

// Passing to Fragment Shader
layout(location = 0) out vec3 color;
layout(location = 1) out vec2 fragTexCoord;

// Main Function
void main() {
    gl_Position = uboViewProjection.projection * uboViewProjection.view * pushConstants.model * vec4(pos, 1.0);
    color = vcolor;
    fragTexCoord = texCoord;
}