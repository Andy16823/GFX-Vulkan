#version 450

// Fragment Input
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

// Fragment Output
layout(location = 0) out vec4 outColor;

// Texture Sampler
layout(set = 1, binding = 0) uniform sampler2D textureSampler;

void main() {
    // Font-Daten sind im R-Kanal (R8 Format)
    float alpha = texture(textureSampler, fragTexCoord).r;
    
    // Weiße Farbe mit Alpha aus der Font-Texture
    outColor = vec4(fragColor, alpha);  // fragColor = (1,1,1), alpha = font data
    
    // Verwerfe transparente Pixel
    if (alpha < 0.01) {
        discard;
    }
}