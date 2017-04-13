#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

uniform mat4 perspective;
uniform mat4 rotation;
uniform mat4 worldRotation = mat4(1.0f);
uniform vec4 light = vec4(50.0f, 100.0f, 50.0f, 0.0f);
uniform float intensity = 1.0f;
uniform vec2 offset;
uniform float z_offset;
uniform mat4 MtoWMatrix;
uniform mat4 WtoCMatrix;
uniform mat4 lightMatrix;
uniform vec3 worldOffset = vec3(0.0f);


const float ambient = 0.5f;

smooth out vec4 out_color;
smooth out vec2 texCoord;
smooth out vec4 lightPos;
smooth out vec4 worldPosition;
smooth out vec3 normals;
smooth out vec4 pos;

void main(void)
{
    vec4 result = vec4(position, 1);
    vec4 worldView = worldRotation * MtoWMatrix * rotation * result + vec4(worldOffset, 0.0f);
    vec4 camView = WtoCMatrix * worldView + vec4(offset, 0.0f, 0.0f);
    mat3 transform = transpose(inverse(mat3(worldRotation * MtoWMatrix * rotation)));
    vec3 rotnormal = normalize(transform * normal);

    out_color = color;
    out_color[3] = 1;


    lightPos = lightMatrix * worldView + vec4(offset, 0.0f, 0.0f);
    gl_Position = perspective * camView;
    texCoord = uv;
    worldPosition = worldView;
    normals = rotnormal;
    pos = camView;
}
