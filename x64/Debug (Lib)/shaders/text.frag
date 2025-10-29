#version 450

// Fragment Input
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

// Fragment Output
layout(location = 0) out vec4 outColor;

// Texture Sampler
layout(set = 1, binding = 0) uniform sampler2D textureSampler;

void main() {
    // Sample font texture
    // Wenn du Swizzle verwendest (R -> A), ist das Alpha bereits richtig
    vec4 sampled = texture(textureSampler, fragTexCoord);
    
    // Nutze den Alpha-Kanal für die Text-Transparenz
    // Farbe kommt von fragColor (weiß), Alpha von der Font-Textur
    outColor = texture(textureSampler, fragTexCoord);
    // outColor = vec4(fragColor, sampled.a);
    
    // Verwerfe komplett transparente Pixel
    if (outColor.a < 0.01) {
        discard;
    }
}