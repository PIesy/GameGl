#version 430

in vec3 worldPosition;

layout(binding = 0) uniform samplerCube colorMap;

out vec4 color;

void main()
{
    color = vec4(texture(colorMap, worldPosition).rgb, 1.0f);
    //color = vec4(vec3(0.0f), 1.0f);
}
