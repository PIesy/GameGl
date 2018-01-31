#ifndef GLUNIFORM_H
#define GLUNIFORM_H

#include "Wrappers/glbindings.h"
#include "../../../Graphics/uniform.h"
#include "../../../Helpers/invokationresult.h"

struct GlUniform: public UniformValue
{
    GLuint location = 0;
};

InvokationResult setUniform(GLuint program, GlUniform& uniform);

#endif // GLUNIFORM_H

