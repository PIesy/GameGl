#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

uniform mat4 perspective;
uniform mat4 rotation;
uniform mat4 worldRotation = mat4(1.0f);
uniform vec4 light;
uniform float intensity;
uniform vec2 offset;
uniform float z_offset;
uniform mat4 MtoWMatrix;
uniform mat4 WtoCMatrix;
uniform vec3 worldOffset = vec3(0.0f);

void main(void)
{
    vec4 result = vec4(position, 1.0f);
    vec4 camView = WtoCMatrix * worldRotation * MtoWMatrix * rotation * result;
    camView = camView; //+ vec4(offset, 0.0f, 0.0f);

    gl_Position = perspective * camView;
}
