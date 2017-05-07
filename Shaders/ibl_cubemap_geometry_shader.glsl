#version 430

layout (invocations = 6) in;
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 rawPosition[];

uniform mat4 projections[6];
uniform mat4 perspective;

out vec3 rawPos;

void main()
{
    gl_Layer = gl_InvocationID;
    for (int i = 0; i < 3; ++i)
    {
        rawPos = rawPosition[i];
        gl_Position = perspective * projections[gl_InvocationID] * gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}