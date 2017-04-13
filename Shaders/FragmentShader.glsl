#version 430


smooth in vec2 texCoord;
smooth in vec4 out_color;
smooth in vec4 lightPos;
smooth in vec4 worldPosition;
smooth in vec4 pos;
smooth in vec3 normals;

uniform bool useTex = false;
uniform vec2 windowSize = vec2(1000.0f, 600.0f);
uniform vec4 light = vec4(50.0f, 100.0f, 50.0f, 0.0f);

layout(binding = 0) uniform sampler2DShadow shadowmap;
layout(binding = 1) uniform sampler2D tex;

const float ambient = 0.5f;
const float shininies = 0.5f;

out vec4 color;

float lightCalc(out vec3 direction, out float angle)
{
    vec3 lightVector = light.xyz - worldPosition.xyz;
    direction = normalize(lightVector);
    vec3 n = normalize(normals);

    float intensity = min(pow(length(lightVector), 2), 1.0f);
    angle = clamp(dot(n, direction), 0.0f, 1.0f);
    return angle / max(intensity, 2.0f);
}

float calcSpecular(in vec3 lightDirection, in float cos)
{
    vec3 viewDir = normalize(-pos.xyz);
    vec3 halfAngle = normalize(lightDirection + viewDir);
    float angle = acos(dot(halfAngle, normalize(normals)));
    float exponent = angle / shininies;
    exponent = -(exponent * exponent);
    float term = 0.0f;
    if (cos != 0.0f)
        term = exp(exponent);
    return term;
}

void main(void)
{
    float visibility = 1.0f;
    vec3 nonHomogeneous = lightPos.xyz / lightPos.w;
    nonHomogeneous = nonHomogeneous * 0.5 + 0.5;
    nonHomogeneous.z = nonHomogeneous.z - 0.00005f;

    color = out_color;
    visibility = clamp(texture(shadowmap, nonHomogeneous.xyz), 0.5f, 1.0f);

    vec3 dir;
    float cos;
    float mul = lightCalc(dir, cos);
    float spec = calcSpecular(dir, cos);
    vec4 temp_color = vec4(0.0f);
    vec4 colr = out_color;
    if (useTex)
        colr = texture(tex, texCoord);
    if (visibility != 1.0f)
        temp_color = colr * ambient * visibility;
    else
        temp_color = colr * ambient + vec4(1.0f) * spec + colr * mul;
    color = vec4(temp_color.xyz, out_color.w);
}
