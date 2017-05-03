#version 430

in vec2 uvCoords;

layout(binding = 0) uniform sampler2D hdrMap;

uniform bool useGammaCorrection = false;
uniform float exposure = 2.0f;

out vec4 color;

void main(void)
{
    vec3 hdrColor = texture(hdrMap, uvCoords).rgb;

    hdrColor = vec3(1.0f) - exp(-hdrColor * exposure);
    if (useGammaCorrection)
        hdrColor = pow(hdrColor, vec3(1.0f / 2.2f));
    color = vec4(hdrColor, 1.0f);
}
