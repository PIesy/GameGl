#ifndef GLUNIFORM_H
#define GLUNIFORM_H

#include "uniform.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "Helpers/invokationresult.h"

struct GlUniform: public UniformValue
{
    GLuint location;
};

InvokationResult setUniform(GLuint program, GlUniform& uniform);

#endif // GLUNIFORM_H

