#version 430

layout(location = 0) in vec3 position;

uniform mat4 MtoWMatrix;
uniform mat4 rotation;

out vec3 rawPosition;

void main()
{
    rawPosition = position;
    gl_Position = vec4(position, 1.0f);
}
