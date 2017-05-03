#version 430

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec3 camNormal[];

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();

        gl_Position = gl_in[i].gl_Position + vec4(camNormal[i], 0.0f) * 10.0f;
        EmitVertex();

        EndPrimitive();
    }
}