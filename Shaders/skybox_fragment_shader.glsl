#version 430

in vec4 worldPosition;

uniform vec3 worldCenter;
layout(binding = 0) uniform samplerCube colorMap;

out vec4 color;

vec4 mapSkybox()
{
    vec3 fragToLight = worldPosition.xyz - worldCenter;
    vec4 color = texture(colorMap, fragToLight);

    return color;
}

void main(void)
{
    vec4 tmp = mapSkybox();
    color = vec4(pow(tmp.xyz, vec3(1.0f / 2.2f)), tmp.w);
}
