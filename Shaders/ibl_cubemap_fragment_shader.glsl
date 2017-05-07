#version 430

in vec4 position;
in vec3 rawPos;

uniform sampler2D hdrMap;

layout(location = 0) out vec4 outColor;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    uv = vec2(uv.x, 1.0f - uv.y);
    return uv;
}

void main()
{
    vec2 uv = SampleSphericalMap(normalize(rawPos));
    vec3 color = texture(hdrMap, uv).rgb;

    outColor = vec4(color, 1.0);
}
