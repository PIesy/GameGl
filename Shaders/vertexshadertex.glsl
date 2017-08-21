#version 430

layout(location = 0) in vec3 position;

uniform mat4 perspective;
uniform mat4 rotation;
uniform mat4 MtoWMatrix;
uniform mat4 WtoCMatrix;

void main(void)
{
    vec4 result = vec4(position, 1.0f);
    vec4 camView = WtoCMatrix * MtoWMatrix * rotation * result;

    gl_Position = perspective * camView;
}
