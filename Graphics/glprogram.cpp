#include "glprogram.h"
#include "renderdefs.h"

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

GlProgram::GlProgram(RenderingContext& context):context(context)
{
    Task create([this] { program = glCreateProgram(); printGlError("Create prog error"); });
    this->context.Execute(create);
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
    Task attach([&glShader, this] { glAttachShader(program, glShader); printGlError("Attach error"); });
    context.Execute(attach);
}

void GlProgram::Detach(const Shader& shader)
{
    const GlShader& glShader = dynamic_cast<const GlShader&>(shader);
    Task detach([&glShader, this] { glDetachShader(program, glShader); });
    context.Execute(detach);
}

GlProgram::operator GLuint() const
{
    return program;
}

void GlProgram::Compile()
{
    Task task([this] { glLinkProgram(program); printGlError("Compile error");});
    context.Execute(task);
    task.WaitTillFinished();
}

void GlProgram::Use()
{
    glUseProgram(program);
    printGlError("Use error");
}
