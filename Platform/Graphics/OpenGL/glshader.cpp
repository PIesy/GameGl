#include "glshader.h"
#include "../../../Graphics/renderdefs.h"
#include "../../../Logger/logger.h"

GlShader::GlShader(RenderingContext& context) : context(context) {}

void GlShader::Create(const std::string& source, ShaderType type)
{
    if (!isValid)
        return;
    this->type = type;
    GLuint shaderType = convertToGlShaderType(type);

    Task create([=]
    {
        shader = gl::shader::create(shaderType);
        gl::shader::setSource(shader, source);
        gl::shader::compile(shader);
        PrintInfo();
    });
    context.Execute(create);
    create.WaitTillFinished();
}

void GlShader::PrintInfo()
{
    if (!isValid)
        return;;
    Task print([this]
    {
        char buff[4096] = {0};
        int length;
        glGetShaderInfoLog(shader, 4095, &length, buff);
        Logger::Log("Shader info: " + std::string(buff));
    });
    context.Execute(print);
}

ShaderType GlShader::GetType() const
{
    return type;
}

GlShader::operator GLuint() const
{
    return shader;
}

GlShader::GlShader(GlShader&& src) : context(src.context)
{
    src.isValid = false;
    shader = src.shader;
    type = src.type;
}
