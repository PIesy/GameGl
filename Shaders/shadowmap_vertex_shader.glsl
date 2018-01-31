#version 430

layout(location = 0) in vec3 position;

uniform mat4 MtoWMatrix;
uniform mat4 rotation;

void main()
{
    gl_Position = MtoWMatrix * rotation * vec4(position, 1.0f);
}
