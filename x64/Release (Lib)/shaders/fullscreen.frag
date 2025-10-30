#version 450
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

// Sampler: set = 0, binding = 0  (passe an dein set-layout an falls anders)
layout(set = 0, binding = 0) uniform sampler2D offscreenSampler;

void main() {
    vec4 tex = texture(offscreenSampler, fragUV);
    float alphaThreshold = 0.1;
    if (tex.a < alphaThreshold) {
        discard;
    }
    // combine texture and vertex color; if you want only texture use tex directly
    outColor = tex * vec4(fragColor, 1.0);
}