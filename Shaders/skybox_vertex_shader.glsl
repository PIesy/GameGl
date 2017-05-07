#version 430

layout(location = 0) in vec3 position;

uniform mat4 perspective;
uniform mat4 WtoCMatrix;

out vec4 worldPosition;

void main(void)
{
    vec3 translatedPos = mat3(WtoCMatrix) * position;
    worldPosition = vec4(position, 1.0f);
    gl_Position = (perspective * vec4(translatedPos, 1.0f)).xyww;
}
