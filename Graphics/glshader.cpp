#include "openglsdl.h"
#include "Logger/logger.h"

struct ShaderData
{
    const char** src;
    int* length;
    std::condition_variable finish;
    GLuint* shader;
    unsigned int type;
};

GlShader::GlShader(Worker *context)
{
    this->context = context;
}

void GlShader::Create(std::string source, ShaderType type)
{
    this->type = type;
    GLuint shaderType = convertToGlShaderType(type);

    Task create([=]
    {
        shader = glCreateShader(shaderType);
        const char* str = source.c_str();
        int size = source.length();
        glShaderSource(shader, 1, &str, &size);
        glCompileShader(shader);
    });
    context->setTask(create);
    create.WaitTillFinished();
}

ShaderType GlShader::getType() const
{
    return type;
}

GlShader::operator GLuint() const
{
    return shader;
}
