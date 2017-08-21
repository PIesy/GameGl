#version 430

layout(location = 0) in vec3 position;
layout(location = 3) in vec2 uv;

uniform mat4 MtoWMatrix;
uniform mat4 rotation;

out vec2 uvCoords;

void main()
{
    uvCoords = uv;
    gl_Position = MtoWMatrix * rotation * vec4(position, 1.0f);
}
