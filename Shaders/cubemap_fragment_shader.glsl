#version 430

in vec4 position;

uniform vec3 lightPosition;
uniform float farPlane;

void main()
{
    float dist = distance(lightPosition, position.xyz) + 1.0f;

    gl_FragDepth = clamp(dist / farPlane, 0.0f, 1.0f);
}
