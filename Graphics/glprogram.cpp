#include "glprogram.h"
#include "renderdefs.h"

GlProgram::GlProgram(RenderingContext& context):context(context)
{
    Task create([this] { program = gl::program::create(); });
    this->context.Execute(create);
}

GlProgram::~GlProgram()
{
    Task remove([this] { gl::program::erase(program); });
    context.Execute(remove);
    remove.WaitTillFinished();
}

void GlProgram::Attach(const Shader& shader)
{
    const GlShader& glShader = dynamic_cast<const GlShader&>(shader);
    Task attach([&glShader, this] { gl::program::attachShader(program, glShader); });
    context.Execute(attach);
}

void GlProgram::Detach(const Shader& shader)
{
    const GlShader& glShader = dynamic_cast<const GlShader&>(shader);
    Task detach([&glShader, this] { gl::program::detachShader(program, glShader); });
    context.Execute(detach);
}

GlProgram::operator GLuint() const
{
    return program;
}

void GlProgram::Compile()
{
    Task task([this]
    {
        gl::program::link(program);
    });
    context.Execute(task);
    task.WaitTillFinished();
}

void GlProgram::Use()
{
    gl::program::use(program);
}

InvokationResult GlProgram::setUniform(const std::string& name, const UniformValue& value)
{
    GlUniform uniform;

    if (!uniforms.count(name))
    {
        Task uniformLoc;
        auto futureLocation = uniformLoc.SetTask([this, &name] {
            return gl::program::getUniformLocation(program, name);
        });
        context.Execute(uniformLoc);
        GLint loc = futureLocation.get();
        if (loc == -1)
            return InvokationResult::ERROR;
        uniform.count = value.count;
        uniform.horizontalSize = value.horizontalSize;
        uniform.type = value.type;
        uniform.verticalSize = value.verticalSize;
        uniform.location = loc;
        uniforms.insert({name, uniform});
    }
    else
        uniform = uniforms[name];
    uniform.value = value.value;

    Task setUniform;
    auto future = setUniform.SetTask([this, &uniform] {
        return ::setUniform(program, uniform);
    });
    context.Execute(setUniform);
    return future.get();
}
