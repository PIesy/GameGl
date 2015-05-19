#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "glshader.h"

class GlProgram: public Program
{
    RenderingContext& context;
    GLuint program = 0;
public:
    GlProgram(RenderingContext& context);
    void Attach(const Shader& shader);
    void Detach(const Shader& shader);
    void Compile();
    void Use();
    operator GLuint() const;
};

#endif // GLPROGRAM_H

