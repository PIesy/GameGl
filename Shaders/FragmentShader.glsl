#version 430

smooth in vec2 texCoord;
smooth in vec4 out_color;
smooth in vec4 worldPosition;
smooth in vec4 pos;
smooth in vec3 normals;

uniform bool useTex = false;
uniform bool useShadow = false;
uniform bool debug = false;
uniform vec3 lightPosition;
uniform vec3 camPosition;
uniform float farPlane;
uniform float nearPlane;

layout(binding = 0) uniform samplerCubeShadow shadowmap;
layout(binding = 1) uniform sampler2D tex;

const float ambient = 0.5f;
const float power = 2000.0f;
const float shininies = 0.1f;

out vec4 color;

float lightCalc(out float intensity, out float angle)
{
    vec3 lightVector = lightPosition - worldPosition.xyz;
    vec3 direction = normalize(lightVector);
    vec3 n = normalize(normals);

    intensity = max(pow(length(lightVector), 2), 1.0f);
    angle = clamp(dot(n, direction), 0.0f, 1.0f);
    return angle * power / intensity;
}

float calcSpecular(float intensity, float cos)
{
    vec3 viewDir = normalize(camPosition - worldPosition.xyz);
    vec3 lightVector = normalize(lightPosition - worldPosition.xyz);
    vec3 halfAngle = normalize(lightVector + viewDir);
    float angle = acos(dot(halfAngle, normalize(normals)));
    float exponent = angle / shininies;
    exponent = -(exponent * exponent);
    float term = 0.0f;
    if (cos != 0.0f)
        term = exp(exponent);
    return term * power / intensity;
}

float calcShadow()
{
    vec3 fragToLight = worldPosition.xyz - lightPosition;
    float currentDepth = length(fragToLight);
    fragToLight = normalize(fragToLight);
    float shadow = 1.0f;
    shadow = texture(shadowmap, vec4(fragToLight, (currentDepth - nearPlane) / (farPlane - nearPlane)));

    return shadow;
}

vec4 debugShadow()
{
    vec3 fragToLight = worldPosition.xyz - lightPosition;
    float currentDepth = length(fragToLight);
    fragToLight = normalize(fragToLight);
    float shadow = 1.0f;//texture(shadowmap, fragToLight).r;

    return vec4(vec3(shadow), 1.0f);
}

void main(void)
{
    vec3 dir;
    float cos, intens;
    float mul = lightCalc(intens, cos);
    float spec = calcSpecular(intens, cos);
    float visibility = 1.0f;

    color = out_color;
    if (useShadow)
        visibility = calcShadow();

    vec4 temp_color = vec4(0.0f);
    vec4 colr = out_color;
    if (useTex)
        colr = texture(tex, texCoord);

    if (debug)
        colr = debugShadow();
    if (visibility != 1.0f)
        temp_color = colr * ambient * visibility;
    else
        temp_color = colr * ambient + vec4(1.0f) * spec + colr * mul;

    color = vec4(temp_color.xyz, out_color.w);
}
