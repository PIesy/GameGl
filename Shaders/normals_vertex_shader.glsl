#version 430

layout(location = 0) in vec3 position;

uniform mat4 perspective;
uniform mat4 rotation;
uniform mat4 MtoWMatrix;
uniform mat4 WtoCMatrix;

void main()
{
    vec4 worldPosition = MtoWMatrix * rotation * vec4(position, 1.0f);

    gl_Position =  perspective * WtoCMatrix * worldPosition;
}