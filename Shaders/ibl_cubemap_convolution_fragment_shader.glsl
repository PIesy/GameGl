#version 430

in vec4 position;
in vec3 rawPos;

layout(binding = 0) uniform samplerCube hdrMap;

layout(location = 0) out vec4 outColor;

const float delta = 0.025f;
const float pi = 3.14159265359;
const float pi2 = 2.0f * pi;
const float halfPi = 0.5f * pi;
const float div = 1.0f / (round(pi2 / delta) * round(halfPi / delta));


void main()
{
    vec3 result = vec3(0.0f);
    vec3 normal = normalize(rawPos);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    for (float phi = 0.0f; phi < pi2; phi += delta)
    {
        for (float theta = 0.0f; theta < halfPi; theta += delta)
        {
            vec3 t = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 vec = t.x * right + t.y * up + t.z * normal;

            vec3 tmpColor = texture(hdrMap, vec).rgb;
            result += tmpColor * cos(theta) * sin(theta) * div;
        }
    }

    result = pi * result;
    outColor = vec4(result, 1.0f);
}
