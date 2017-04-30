#ifndef GLSHADER_H
#define GLSHADER_H

#include "Wrappers/glbindings.h"
#include "../../../Graphics/graphicsclasses.h"

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

