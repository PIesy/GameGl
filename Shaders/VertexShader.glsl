#version 430

layout(location = 0) in vec3 position;

uniform mat4 perspective;
uniform mat4 WtoCMatrix;
uniform mat4 projections[6];

out vec3 rawPos;

void main()
{
    rawPos = position;
    gl_Position = perspective * projections[0] * vec4(position, 1.0f);
}
