#version 430

in vec2 uvCoords;
in vec3 worldPosition;
in vec3 normalWorldDirection;
in mat3 normalTransform;

layout(binding = 0) uniform samplerCube irradianceMap;
layout(binding = 1) uniform samplerCube prefilterMap;
layout(binding = 2) uniform sampler2D brdfMap;
layout(binding = 3) uniform samplerCubeArrayShadow shadowMaps;
layout(binding = 4) uniform sampler2DArrayShadow globalShadowMaps;
layout(binding = 5) uniform sampler2DArray material;
layout(binding = 6) uniform sampler2D aoMap;

uniform int tilingFactor = 1;
uniform bool albedoTex = false;
uniform vec3 albedo = vec3(0.5f, 0.0f, 0.0f);
uniform bool metallnessTex = false;
uniform float metallness = 0.0f;
uniform bool roughnessTex = false;
uniform float roughness = 1.0f;
uniform bool aoTex = false;
uniform float ao = 1.0f;
uniform bool aoTiling = false;
uniform bool normalTex = false;
uniform bool inverseRoughness = false;
uniform bool noIbl = false;
uniform bool noShadows = false;
uniform bool noGlobalShadows = false;

uniform vec3 lightPositions[256];
uniform vec3 lightColors[256];
uniform float lightFarPlanes[256];
uniform int actualLightsCount = 0;
uniform vec3 directionalLights[256];
uniform vec3 directionalLightsColor[256];
uniform mat4 directionalLightsMatrices[256];
uniform int directionalLightsCount = 0;

uniform vec3 camPosition;

const float pi = 3.14159265359;
const float powerMultiplier = 1000.0f;

out vec4 outColor;

float GGXDistribution(vec3 normal, vec3 halfVector, float roughness)
{
    float a2 = pow(roughness, 4);
    float NdotH2 = pow(max(dot(normal, halfVector), 0.0), 2);

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = pi * denom * denom;

    return a2 / denom;
}

float geometrySchlickGGX(float NV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    return NV / (NV * (1.0 - k) + k);
}

vec3 fresnel(vec3 F0, float cosine)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosine, 5.0);
}

vec3 fresnelRough(vec3 F0, float cosine, float roughness)
{
    return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(1.0 - cosine, 5.0);
}

vec3 calculateLight(vec3 normal, vec3 viewDirection, vec3 F0, vec3 lightPosition, vec3 lightColor, vec3 a, float r, float m)
{
    vec3 lightDirection = normalize(lightPosition - worldPosition);
    vec3 halfVector = normalize(viewDirection + lightDirection);
    float NV = clamp(dot(normal, viewDirection), 0.0f, 1.0f);
    float NL = clamp(dot(normal, lightDirection), 0.0f, 1.0f);
    float HV = clamp(dot(halfVector, viewDirection), 0.0f, 1.0f);
    float distance = length(lightPosition - worldPosition);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lightColor * attenuation * powerMultiplier;

    float NDF = GGXDistribution(normal, halfVector, r);
    float G = geometrySchlickGGX(NV, r) * geometrySchlickGGX(NL, r);
    vec3 kS = fresnel(F0, HV);

    vec3 specular = NDF * G * kS / (4 * NV * NL + 0.001);
    vec3 kD = (vec3(1.0) - kS) * (1 - m);

    return (kD * a / pi + specular) * radiance * NL;
}

vec3 calculateDirectionalLight(vec3 normal, vec3 viewDirection, vec3 F0, vec3 direction, vec3 lightColor, vec3 a, float r, float m)
{
    vec3 lightDirection = normalize(-direction);
    vec3 halfVector = normalize(viewDirection + lightDirection);
    float NV = clamp(dot(normal, viewDirection), 0.0f, 1.0f);
    float NL = clamp(dot(normal, lightDirection), 0.0f, 1.0f);
    float HV = clamp(dot(halfVector, viewDirection), 0.0f, 1.0f);
    vec3 radiance = lightColor;

    float NDF = GGXDistribution(normal, halfVector, r);
    float G = geometrySchlickGGX(NV, r) * geometrySchlickGGX(NL, r);
    vec3 kS = fresnel(F0, HV);

    vec3 specular = NDF * G * kS / (4 * NV * NL + 0.001);
    vec3 kD = (vec3(1.0) - kS) * (1 - m);

    return (kD * a / pi + specular) * radiance * NL;
}

float checkShadows(int i)
{
    if (noShadows)
        return 1.0f;
    vec3 lightDir = worldPosition - lightPositions[i];
    float depth = distance(lightPositions[i], worldPosition);
    float shadow = texture(shadowMaps, vec4(lightDir, i), depth / lightFarPlanes[i]);
    return shadow;
}

float checkGlobalShadows(int i)
{
    if (noGlobalShadows)
        return 1.0f;
    vec4 projection = directionalLightsMatrices[i] * vec4(worldPosition, 1.0f);
    projection.xyz = projection.xyz * 0.5f + 0.5f;
    projection.z -= 0.001f;
    float shadow = texture(globalShadowMaps, vec4(projection.xy, i, projection.z));
    return shadow;
}

void main()
{
    vec3 viewDirection = normalize(camPosition - worldPosition);
    vec3 light = vec3(0.0);
    vec3 F0 = vec3(0.04);
    vec2 modUv = uvCoords * tilingFactor;

    vec3 normal = normalWorldDirection;
    int index = 0;
    if (normalTex)
        normal = texture(material, vec3(modUv, index++)).rgb;
    normal = normalize(normal * 2.0f - 1.0f);
    normal = normalize(normalTransform * normal);

    vec3 a = albedo;
    if (albedoTex)
    {
        a = texture(material, vec3(modUv, index)).rgb;
        index++;
    }

    float m = metallness;
    if (metallnessTex)
    {
        m = texture(material, vec3(modUv, index)).r;
        index++;
    }

    float r = roughness;
    if (roughnessTex)
    {
        r = texture(material, vec3(modUv, index)).r;
        if (inverseRoughness)
            r = 1.0f - r;
    }

    float ao_ = ao;
    if (aoTex)
        ao_ = texture(aoMap, aoTiling ? modUv : uvCoords).r;

    F0 = mix(F0, a, m);

    for (int i = 0; i < actualLightsCount; ++i)
    {
        light += calculateLight(normal, viewDirection, F0, lightPositions[i], lightColors[i], a, r, m) * checkShadows(i);
    }

    for (int i = 0; i < directionalLightsCount; ++i)
    {
        light += calculateDirectionalLight(normal, viewDirection, F0, directionalLights[i], directionalLightsColor[i], a, r, m) * checkGlobalShadows(i);
    }

    vec3 ambient = vec3(0.0f);

    vec3 spec = vec3(0.0f);
    vec3 diffuse = vec3(0.1f);
    vec3 kS = vec3(0.7f);

    float NV = max(dot(normal, viewDirection), 0.0f);
    kS = fresnelRough(F0, NV, r);
    diffuse = texture(irradianceMap, normal).rgb * a;


    vec3 refl = reflect(-viewDirection, normal);
    float mipmapCount = textureQueryLevels(prefilterMap);
    vec3 pref = textureLod(prefilterMap, refl, r * mipmapCount).rgb;

    vec3 fresn = fresnelRough(F0, NV, r);
    vec2 brdf = texture(brdfMap, vec2(NV, r)).rg;
    spec = pref * (fresn * brdf.x + brdf.y);

    ambient = ((1.0f - kS) * diffuse + spec) * ao_;

    vec3 color = ambient + light;
    outColor = vec4(color, 1.0);
}
