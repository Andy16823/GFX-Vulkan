#version 450
//////////////////////////////////////////////////////////////////
// Sets:
// 0: Camera UBO
// 1: Instance data (storage buffer)
// 2: Albedo Map
// 3: Normal Map
// 4: Metallic-Roughness Map
// 5: AO Map
// 6: PBR Material Properties UBO
// 7: (Unused)
//////////////////////////////////////////////////////////////////

// Fragment input from vertex shader
layout(location = 0) in vec3 color;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec4 fragExtras;
layout(location = 4) in vec3 fragWorldPos;

layout(set = 6, binding = 0) uniform PBRMaterialProperties {
    vec4 albedoColor;
} materialProps;

layout(set = 7, binding = 0) uniform DirectionalLightData {
    vec4 direction;
    vec4 colorIntensity;
} dirLight;

// Camera UBO
layout(set = 0, binding = 0) uniform UboViewProjection {
    mat4 projection;
    mat4 view;
    vec3 cameraPos;
} uboViewProjection;


// Texture sampler
layout(set = 2, binding = 0) uniform sampler2D albedoMap;
layout(set = 3, binding = 0) uniform sampler2D normalMap;
layout(set = 4, binding = 0) uniform sampler2D metallicRoughnessMap;
layout(set = 5, binding = 0) uniform sampler2D aoMap;

layout(location = 0) out vec4 outColor;

// ═══════════════════════════════════════════════════════════════
// PBR CONSTANTS
// ═══════════════════════════════════════════════════════════════
const float PI = 3.14159265359;


// ═══════════════════════════════════════════════════════════════
// PBR FUNCTIONS
// ═══════════════════════════════════════════════════════════════

// Normal Distribution Function (GGX/Trowbridge-Reitz)
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.0000001);
}

// Geometry Function (Schlick-GGX)
float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / max(denom, 0.0000001);
}

// Smith's method
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Fresnel-Schlick approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// ═══════════════════════════════════════════════════════════════
// MAIN PBR LIGHTING
// ═══════════════════════════════════════════════════════════════
void main() {
    // ✅ Check visibility (from extras.x)
    float visible = fragExtras.x;
    if (visible < 0.5) {
        discard;
    }

    // Directional light parameters
    vec3 lightDir = dirLight.direction.xyz;
    vec3 lightColor = dirLight.colorIntensity.rgb;
    float lightIntensity = dirLight.colorIntensity.a;

    // Sample material properties
    vec4 albedoTextureColor = texture(albedoMap, fragTexCoord);
    vec4 albedoColor = materialProps.albedoColor * albedoTextureColor;
    vec3 albedo = pow(albedoColor.rgb, vec3(2.2));  // sRGB to linear
    
    // ✅ Sample combined metallic-roughness texture
    vec3 metallicRoughnessAO = texture(metallicRoughnessMap, fragTexCoord).rgb;
    float roughness = metallicRoughnessAO.g;  // Green channel
    float metallic = metallicRoughnessAO.b;   // Blue channel
    
    // ✅ Sample ambient occlusion
    float ao = texture(aoMap, fragTexCoord).r;
    
    // ✅ Get normal from vertex shader (normal mapping später)
    vec3 N = normalize(fragNormal);
    
    // ✅ Calculate view direction
    vec3 V = normalize(uboViewProjection.cameraPos - fragWorldPos);
    
    // ✅ Light direction (negative because pointing TO light)
    vec3 L = -lightDir;
    vec3 H = normalize(V + L);
    
    // ✅ Calculate radiance
    vec3 radiance = lightColor * lightIntensity;
    
    // ✅ Calculate F0 (base reflectivity)
    vec3 F0 = vec3(0.04);  // Dielectric default (4%)
    F0 = mix(F0, albedo, metallic);  // Metals use albedo as F0
    
    // ═══════════════════════════════════════════════════════════════
    // COOK-TORRANCE BRDF
    // ═══════════════════════════════════════════════════════════════
    
    // ✅ Calculate specular reflection
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
    
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;
    
    // ✅ Energy conservation
    vec3 kS = F;  // Specular contribution
    vec3 kD = vec3(1.0) - kS;  // Diffuse contribution
    kD *= 1.0 - metallic;  // Metals have no diffuse
    
    // ✅ Lambert diffuse
    float NdotL = max(dot(N, L), 0.0);
    vec3 diffuse = kD * albedo / PI;
    
    // ✅ Combine direct lighting
    vec3 Lo = (diffuse + specular) * radiance * NdotL;
    
    // ✅ Ambient lighting (simple approximation)
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    // ✅ Final color
    vec3 finalColor = ambient + Lo;
    
    // ═══════════════════════════════════════════════════════════════
    // TONE MAPPING & GAMMA CORRECTION
    // ═══════════════════════════════════════════════════════════════
    
    // ✅ HDR tone mapping (Reinhard)
    finalColor = finalColor / (finalColor + vec3(1.0));
    
    // ✅ Gamma correction (linear to sRGB)
    finalColor = pow(finalColor, vec3(1.0 / 2.2));
    
    outColor = vec4(finalColor, 1.0);
}