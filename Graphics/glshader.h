#ifndef GLSHADER_H
#define GLSHADER_H

#include "graphicsclasses.h"
#include <GL/glew.h>
#include <GL/gl.h>

class GlShader: public Shader
{
    RenderingContext& context;
    GLuint shader = 0;
    ShaderType type;
public:
    GlShader(RenderingContext& context);
    void Create(std::string source, ShaderType type);
    void PrintInfo();
    ShaderType getType() const;    
    operator GLuint() const;
};

#endif // GLSHADER_H

