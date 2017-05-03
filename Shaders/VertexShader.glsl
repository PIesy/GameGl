#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

uniform mat4 perspective;
uniform mat4 rotation;
uniform mat4 MtoWMatrix;
uniform mat4 WtoCMatrix;

const float ambient = 0.5f;

smooth out vec4 out_color;
smooth out vec2 texCoord;
smooth out vec4 worldPosition;
smooth out vec3 normals;
smooth out vec4 pos;

void main(void)
{
    vec4 result = vec4(position, 1);
    vec4 worldView = MtoWMatrix * rotation * result;
    vec4 camView = WtoCMatrix * worldView;
    mat3 transform = transpose(inverse(mat3(MtoWMatrix * rotation)));
    vec3 rotnormal = normalize(transform * normal);

    out_color = color;
    out_color[3] = 1;

    gl_Position = perspective * camView;
    texCoord = uv;
    worldPosition = worldView;
    normals = rotnormal;
    pos = camView;
}
