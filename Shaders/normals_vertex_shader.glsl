#version 430

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normal;

out vec3 camNormal;

uniform mat4 perspective;
uniform mat4 rotation;
uniform mat4 MtoWMatrix;
uniform mat4 WtoCMatrix;

void main()
{
    mat3 transform = transpose(inverse(mat3(MtoWMatrix * rotation)));
    vec3 rotnormal = transform * normal;
    vec4 worldPosition = MtoWMatrix * rotation * vec4(position, 1.0f);

    camNormal = normalize(vec3(perspective * WtoCMatrix * vec4(rotnormal, 1.0f)));
    gl_Position =  perspective * WtoCMatrix * worldPosition;
}