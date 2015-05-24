#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 perspective;
uniform vec2 offset = vec2(0.5f, 0.5f);

smooth out vec4 out_color;

void main(void)
{
    vec4 result = position + vec4(offset.x, offset.y, 0, 0);
    out_color = color;
    gl_Position = perspective * result;
}
