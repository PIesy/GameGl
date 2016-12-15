#include "glprogram.h"
#include "renderdefs.h"
#include "gluniform.h"

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
//        offsetLoc = glGetUniformLocation(program, "offset");
//        printGlError("Uniform location error");
//        perspectiveLoc = glGetUniformLocation(program, "perspective");
//        rotationLoc = glGetUniformLocation(program, "rotation");
        lightLoc = gl::program::getUniformLocation(program, "light");
        intensityLoc = gl::program::getUniformLocation(program, "intensity");
    });
    context.Execute(task);
    task.WaitTillFinished();
}

void GlProgram::Use()
{
    gl::program::use(program);
    //printGlError("Use error");
    //glUniform2fv(offsetLoc, 1, glm::value_ptr(offset));
    //printGlError("Uniform error");
    //glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, glm::value_ptr(perspective));
    //printGlError("Uniform error");
    //glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(rotation));
    //gl::program::setUniform4f(lightLoc, 1, glm::value_ptr(light));
    //glUniform1f(intensityLoc, intensity);
}

void GlProgram::SetOffset(Vec2 offset)
{
    this->offset = offset;
}

void GlProgram::SetPerspective(Mat4 perspective)
{
    this->perspective = perspective;
}

void GlProgram::SetRotation(Mat4 rotation)
{
    this->rotation = rotation;
}

void GlProgram::SetLight(Vec4 light)
{
    this->light = light;
}

void GlProgram::SetIntensity(float intensity)
{
    this->intensity = intensity;
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
