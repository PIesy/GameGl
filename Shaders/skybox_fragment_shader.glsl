#version 430

in vec4 worldPosition;

layout(binding = 0) uniform samplerCube colorMap;

out vec4 color;

void main()
{
    color = texture(colorMap, worldPosition.xyz);
}
