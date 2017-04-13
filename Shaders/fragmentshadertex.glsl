#version 430

layout(location = 0) out float depth;

smooth in vec4 oColor;
smooth in vec4 pos;

void main(void)
{
    depth = gl_FragCoord.z;

    //depth = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0f);
}
