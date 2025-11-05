#version 450
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vcolor;

layout(set = 0, binding = 0) uniform UboViewProjection {
    mat4 projection;
    mat4 view;
} ubo;

layout(push_constant) uniform PushConstants {
    mat4 model;
    vec4 color;
} pushConstants;

layout(location = 0) out vec4 color;


void main() {
    gl_Position = ubo.projection * ubo.view * pushConstants.model * vec4(pos, 1.0);
    color = pushConstants.color;
}
