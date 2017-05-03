#version 430

in vec4 position;

uniform vec3 lightPosition;
uniform float farPlane;
uniform float nearPlane;

layout(location = 0) out vec4 outColor;

void main()
{
    float dist = distance(lightPosition, position.xyz) + 0.50f;

    outColor = vec4(1, 1, 1, 1);
    switch (gl_Layer)
    {
        case 0:
            outColor = vec4(0, 1, 0, 1);
            break;
        case 1:
            outColor = vec4(1, 0, 0, 1);
            break;
        case 2:
            outColor = vec4(0, 0, 1, 1);
            break;
        case 3:
            outColor = vec4(1, 0, 1, 1);
            break;
        case 4:
            outColor = vec4(0, 1, 1, 1);
            break;
        case 5:
            outColor = vec4(1, 1, 0, 1);
            break;
    }
    gl_FragDepth = clamp((dist - nearPlane) / (farPlane - nearPlane), 0.0f, 1.0f);
}
