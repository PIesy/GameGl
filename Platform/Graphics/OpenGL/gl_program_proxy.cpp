//
// Created by akann on 10/6/17.
//

#include "gl_program_proxy.h"

ShaderType GlProgramProxy::GetType() const
{
    Task getType;
    auto future = getType.SetTask([&, this]
    {
        return handle.GetType();
    });
    context.Execute(getType);
    return future.get();
}

void GlProgramProxy::SetUniform(const std::string& name, const UniformValue& value)
{
    Task setUniform{[=]
    {
        handle.SetUniform(name, value);
    }};
    context.Execute(setUniform);
    setUniform.WaitTillFinished();
}

GlProgramProxy::GlProgramProxy(GlProgramHandle handle, SdlGLContext& context) : handle(std::move(handle)), context(context) {}

const gl::GlProgram& GlProgramProxy::GetProgram() const
{
    return handle.GetProgram();
}

size_t GlProgramProxy::GetHandle() const
{
    return handle.GetHandle();
}
