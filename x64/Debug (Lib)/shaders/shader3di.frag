#version 450

// Fragment input from vertex shader
layout(location = 0) in vec3 color;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec4 fragExtras;

// Texture sampler
layout(set = 2, binding = 0) uniform sampler2D textureSampler;

// Fragment Output
layout(location = 0) out vec4 fragColor;

void main() {
    float visible = fragExtras.x;

    // Discard if not visible
    if(visible<0.5) {
        discard;
    }

    // Sample texture
    fragColor = texture(textureSampler, fragTexCoord);
}