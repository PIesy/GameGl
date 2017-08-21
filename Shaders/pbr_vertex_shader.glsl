#version 430

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;
layout(location = 4) in vec3 tangent;

out vec2 uvCoords;
out vec3 worldPosition;
out vec3 normalWorldDirection;
out mat3 normalTransform;

uniform mat4 pMatrix;
uniform mat4 rotation;
uniform mat4 MtoWMatrix;

void main()
{
    mat3 transform = mat3(MtoWMatrix * rotation);
    vec3 tangentWorld = normalize(transform * tangent);

    worldPosition = vec3(MtoWMatrix * rotation * vec4(position, 1.0f));
    normalWorldDirection = normalize(transform * normal);
    tangentWorld = normalize(tangentWorld - dot(normalWorldDirection, tangentWorld) * normalWorldDirection);
    normalTransform = mat3(tangentWorld, cross(normalWorldDirection, tangentWorld), normalWorldDirection);
    uvCoords = uv;

    gl_Position =  pMatrix * vec4(worldPosition, 1.0);
}