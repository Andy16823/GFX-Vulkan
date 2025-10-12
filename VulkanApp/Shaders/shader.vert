#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vcolor;
layout(location = 2) in vec2 texCoord;

layout(set = 0, binding = 0) uniform UboViewProjection {
    mat4 projection;
    mat4 view;
} uboViewProjection;

// not in use anymore
layout(set = 0, binding = 1) uniform UboModel {
    mat4 model;
} uboModel;

layout(push_constant) uniform PushConstants {
    mat4 model;
} pushConstants;

layout(location = 0) out vec3 color;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    gl_Position = uboViewProjection.projection * uboViewProjection.view * pushConstants.model * vec4(pos, 1.0);
    color = vcolor;
    fragTexCoord = texCoord;
}