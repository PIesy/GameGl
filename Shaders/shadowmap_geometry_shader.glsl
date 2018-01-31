#version 430

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 tMatrix;
uniform int baseLayer;

void main()
{
    gl_Layer = baseLayer;
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = tMatrix * gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}