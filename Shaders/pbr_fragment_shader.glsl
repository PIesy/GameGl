#version 430

in vec2 uvCoords;
in vec3 worldPosition;
in vec3 normalWorldDirection;
in mat3 normalTransform;

layout(binding = 0) uniform samplerCube irradianceMap;
layout(binding = 1) uniform samplerCube prefilterMap;
layout(binding = 2) uniform sampler2D brdfMap;
layout(binding = 3) uniform samplerCubeArrayShadow shadowMaps;
layout(binding = 4) uniform sampler2DArray material;

uniform vec3 albedo = vec3(0.5f, 0.0f, 0.0f);
uniform float metallic = 0.5f;
uniform float roughness = 0.1f;
uniform float ao = 1.0f;

uniform vec3 lightPositions[256];
uniform vec3 lightColors[256];
uniform float lightFarPlanes[256];
uniform int actualLightsCount;

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

bool checkShadows(int i)
{
    vec3 lightDir = worldPosition - lightPositions[i];
    float depth = length(lightDir);
    float shadow = texture(shadowMaps, vec4(lightDir, i), depth / lightFarPlanes[i]);
    return shadow < 1;
}

void main()
{
    vec3 viewDirection = normalize(camPosition - worldPosition);
    vec3 light = vec3(0.0);
    vec3 F0 = vec3(0.04);

    vec3 normal = texture(material, vec3(uvCoords, 0)).rgb;
    normal = normalize(normal * 2.0f - 1.0f);
    normal = normalize(normalTransform * normal);
    vec3 a = texture(material, vec3(uvCoords, 1)).rgb;
    float m = texture(material, vec3(uvCoords, 2)).r;
    float r = texture(material, vec3(uvCoords, 3)).r;
    F0 = mix(F0, a, m);

    for (int i = 0; i < actualLightsCount; ++i)
    {
        if (checkShadows(i))
            light += calculateLight(normal, viewDirection, F0, lightPositions[i], lightColors[i], a, r, m);
    }



    float NV = max(dot(normal, viewDirection), 0.0f);
    vec3 kS = fresnelRough(F0, NV, r);
    vec3 diffuse = texture(irradianceMap, normal).rgb * a;


    vec3 refl = reflect(-viewDirection, normal);
    float mipmapCount = textureQueryLevels(prefilterMap);
    vec3 pref = textureLod(prefilterMap, refl, r * mipmapCount).rgb;

    vec3 fresn = fresnelRough(F0, NV, r);
    vec2 brdf = texture(brdfMap, vec2(NV, r)).rg;
    vec3 spec = pref * (fresn * brdf.x + brdf.y);

    vec3 ambient = ((1.0f - kS) * diffuse + spec) * ao;

    vec3 color = ambient + light;
    outColor = vec4(color, 1.0);
}
