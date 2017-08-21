#version 430

layout(location = 0) in vec3 position;
layout(location = 3) in vec2 uv;

out vec2 uvCoords;
out vec3 rawPosition;

void main()
{
    uvCoords = uv;
    rawPosition = position;
    gl_Position = vec4(position, 1.0f);
}
