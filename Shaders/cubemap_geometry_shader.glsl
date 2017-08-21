#version 430

layout (invocations = 6) in;
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 projections[6];
uniform mat4 tMatrix;
uniform int baseLayer;

out vec4 position;

void main()
{
    gl_Layer = baseLayer * 6 + gl_InvocationID;
    for (int i = 0; i < 3; ++i)
    {
        position = gl_in[i].gl_Position;
        gl_Position = tMatrix * projections[gl_InvocationID] * position;
        EmitVertex();
    }
    EndPrimitive();
}