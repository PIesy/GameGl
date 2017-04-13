#include "glshader.h"
#include "renderdefs.h"
#include "../Logger/logger.h"

GlShader::GlShader(RenderingContext& context):context(context) {}

void GlShader::Create(std::string source, ShaderType type)
{
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
    Task print([this]
    {
        char buff[4096] = {0};
        int length;
        glGetShaderInfoLog(shader, 4095, &length, buff);
        Logger::Log("Shader info: " + std::string(buff));
    });
    context.Execute(print);
}

ShaderType GlShader::getType() const
{
    return type;
}

GlShader::operator GLuint() const
{
    return shader;
}
