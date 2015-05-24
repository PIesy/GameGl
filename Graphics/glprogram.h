#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "glshader.h"

class GlProgram: public Program
{
    RenderingContext& context;
    GLuint program = 0;
    Vec2 offset;
    Mat4 perspective;
    GLuint offsetLoc;
    GLuint perspectiveLoc;
public:
    ~GlProgram();
    GlProgram(RenderingContext& context);
    void Attach(const Shader& shader);
    void Detach(const Shader& shader);
    void Compile();
    void Use();
    void SetOffset(Vec2 offset);
    void SetPerspective(Mat4 perspective);
    operator GLuint() const;
};

#endif // GLPROGRAM_H

