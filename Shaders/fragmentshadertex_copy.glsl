#version 330


smooth in vec2 texCoord;
smooth in vec4 out_color;

uniform bool useTex = false;

uniform sampler2D texture2D;

layout(location = 0) out vec4 color;

void main(void)
{
    color = out_color;
    if (useTex)
        color = texture(texture2D, texCoord);
}
