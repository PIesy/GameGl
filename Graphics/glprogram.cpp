#include "openglsdl.h"
#include <cstdio>
#include "Logger/logger.h"

struct ShaderProgram
{
    Shader* shader;
    GLuint program;
};

struct CompileData
{
    std::condition_variable var;
    GLuint* program;
};

GlProgram::GlProgram(Worker *context)
{
    Task create([this] { program = glCreateProgram(); });
    this->context = context;
    this->context->setTask(create);
    create.WaitTillFinished();
}

//GlProgram::~GlProgram()
//{
//    Task remove([this] { glDeleteProgram(program); });
//    context->setTask(remove);
//    remove.WaitTillFinished();
//}

void GlProgram::Attach(const Shader& shader)
{
    const GlShader& glShader = dynamic_cast<const GlShader&>(shader);
    Task attach([&glShader, this] { glAttachShader(program, glShader); });
    context->setTask(attach);
    attach.WaitTillFinished();
}

void GlProgram::Detach(const Shader& shader)
{
    const GlShader& glShader = dynamic_cast<const GlShader&>(shader);
    Task detach([&glShader, this] { glDetachShader(program, glShader); });
    context->setTask(detach);
    detach.WaitTillFinished();
}

GlProgram::operator GLuint() const
{
    return program;
}

void GlProgram::Compile()
{
    Task task([this] { glLinkProgram(program); });
    context->setTask(task);
    task.WaitTillFinished();
}

void GlProgram::Use()
{
    glUseProgram(program);
}
