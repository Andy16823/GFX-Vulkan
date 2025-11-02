#version 450

//////////////////////////////////////////////////////////////////
// Unlit 3D Fragment Shader
//////////////////////////////////////////////////////////////////

// Fragment Input
layout(location = 0) out vec4 fragColor;
layout(location = 0) in vec3 color;
layout(location = 1) in vec2 fragTexCoord;

// Texture Sampler
layout(set = 1, binding = 0) uniform sampler2D textureSampler;

// Main Function
void main() { 
    fragColor = texture(textureSampler, fragTexCoord);
}