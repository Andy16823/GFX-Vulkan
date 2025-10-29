#version 450

// Vertex Input
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

// Vertex Output
layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

// Uniform Buffer (View/Projection Matrix)
layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 projection;
} ubo;

// Push Constants (Model Matrix)
layout(push_constant) uniform PushConstants {
    mat4 model;
} push;

void main() {
    // Transform vertex position
    gl_Position = ubo.projection * ubo.view * vec4(inPosition, 1.0);
    
    // Pass color and texture coordinates to fragment shader
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}