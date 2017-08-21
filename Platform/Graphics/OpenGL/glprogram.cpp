#include "glprogram.h"
#include "../../../Graphics/renderdefs.h"

GlProgram::GlProgram(RenderingContext& context):context(context)
{
    Task create([this] { program = gl::program::create(); });
    this->context.Execute(create);
}

GlProgram::~GlProgram()
{
    if (!isValid)
        return;
    Task remove([this] { gl::program::erase(program); });
    context.Execute(remove);
    remove.WaitTillFinished();
}

void GlProgram::Attach(const Shader& shader)
{
    if (!isValid)
        return;
    const GlShader& glShader = dynamic_cast<const GlShader&>(shader);
    Task attach([&glShader, this] { gl::program::attachShader(program, glShader); });
    context.Execute(attach);
}

void GlProgram::Detach(const Shader& shader)
{
    if (!isValid)
        return;
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
    if (!isValid)
        return;
    Task task([this]
    {
        gl::program::link(program);
    });
    context.Execute(task);
    task.WaitTillFinished();
}

void GlProgram::Use()
{
    if (!isValid)
        return;
    gl::program::use(program);
}

InvokationResult GlProgram::SetUniform(const std::string& name, const UniformValue& value)
{
    if (!isValid)
        return InvokationResult::ERROR;
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

GlProgram::GlProgram(GlProgram&& src) : context(src.context)
{
    src.isValid = false;
    program = src.program;
    uniforms = std::move(src.uniforms);
}
