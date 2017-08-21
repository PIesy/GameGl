#version 430

in vec2 uvCoords;
in vec3 rawPosition;

layout(binding = 0) uniform samplerCube hdrMap;

out vec4 color;

void main(void)
{
    vec3 hdrColor = texture(hdrMap, rawPosition).rgb;

    color = vec4(hdrColor, 1.0f);
}
