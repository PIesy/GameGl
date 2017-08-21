#ifndef GLSHADER_H
#define GLSHADER_H

#include "Wrappers/glbindings.h"
#include "../../../Graphics/graphicsclasses.h"

class GlShader: public Shader
{
    RenderingContext& context;
    GLuint shader = 0;
    ShaderType type;
    bool isValid = true;
public:
    explicit GlShader(RenderingContext& context);
    GlShader(const GlShader&) = delete;
    GlShader(GlShader&& src);
    void Create(const std::string& source, ShaderType type);
    void PrintInfo();
    ShaderType GetType() const override;
    operator GLuint() const;
};

#endif // GLSHADER_H

