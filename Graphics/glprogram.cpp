#include "openglsdl.h"
#include <cstdio>

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

void glNewProgram(CompileData* program)
{
    *program->program = glCreateProgram();
    program->var.notify_all();
}

GlProgram::GlProgram(Worker *context)
{
    CompileData data;
    std::unique_lock<std::mutex> wait(lock);

    data.program = &program;
    this->context = context;
    this->context->setTask(Task(glNewProgram, &data));
    data.var.wait(wait);
}

void glDropProgram(GLuint* program)
{
    glDeleteProgram(*program);
    delete program;
}

GlProgram::~GlProgram()
{
    GLuint* prog = new GLuint(program);
    context->setTask(Task(glDropProgram, prog));
}

void glShaderAttach(ShaderProgram* prog)
{
    glAttachShader(prog->program, *prog->shader);
    delete prog;
}

void GlProgram::Attach(Shader* shader)
{
    ShaderProgram* prog = new ShaderProgram;
    prog->shader = shader;
    prog->program = program;
    context->setTask(Task(glShaderAttach, prog));
}

void glShaderDetach(ShaderProgram* prog)
{
    glDetachShader(prog->program, *prog->shader);
    delete prog;
}

void GlProgram::Detach(Shader* shader)
{
    ShaderProgram* prog = new ShaderProgram;
    prog->shader = shader;
    prog->program = program;
    context->setTask(Task(glShaderDetach, prog));
}

GlProgram::operator GLuint()
{
    return program;
}

void glProgramLink(CompileData* program)
{
    glLinkProgram(*program->program);
    program->var.notify_all();
}

void GlProgram::Compile()
{
    CompileData data;
    std::unique_lock<std::mutex> wait(lock);
    data.program = &program;
    context->setTask(Task(glProgramLink, &data));
    data.var.wait(wait);
}

void GlProgram::Use(void*)
{
    glUseProgram(program);
}
