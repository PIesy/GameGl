#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;

uniform mat4 perspective;
uniform mat4 rotation;
uniform vec4 light;
uniform float intensity;
uniform vec2 offset;
uniform float z_offset;

const float ambient = 0.5f;

smooth out vec4 out_color;

void main(void)
{
    vec3 rotnormal = normalize(mat3(rotation) * normal);
    vec4 result = rotation * vec4(position, 1);
    float angle = dot(light.xyz, rotnormal);
    angle = clamp(angle, 0, 1);
    result += vec4(offset.x, offset.y, z_offset, 0);
    out_color = intensity * color * angle + color * ambient;
    out_color[3] = 1;
    gl_Position = perspective * result;
}
