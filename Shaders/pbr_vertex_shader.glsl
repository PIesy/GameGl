#version 430

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normal;

out vec2 uvCoords;
out vec3 worldPosition;
out vec3 normalWorldDirection;

uniform mat4 perspective;
uniform mat4 rotation;
uniform mat4 MtoWMatrix;
uniform mat4 WtoCMatrix;

void main()
{
    mat3 transform = transpose(inverse(mat3(MtoWMatrix * rotation)));
    vec3 rotnormal = normalize(transform * normal);

    worldPosition = vec3(MtoWMatrix * rotation * vec4(position, 1.0f));
    normalWorldDirection = rotnormal;

    gl_Position =  perspective * WtoCMatrix * vec4(worldPosition, 1.0);
}