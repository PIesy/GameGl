#version 430

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 tMatrix;
uniform int baseLayer;

out vec4 position;

void main()
{
    gl_Layer = baseLayer;
    for (int i = 0; i < 3; ++i)
    {
        position = gl_in[i].gl_Position;
        gl_Position = tMatrix * position;
        EmitVertex();
    }
    EndPrimitive();
}