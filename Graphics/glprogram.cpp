#include "glprogram.h"
#include "renderdefs.h"
#include "gluniform.h"

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

GlProgram::~GlProgram()
{
    Task remove([this] { glDeleteProgram(program); });
    context.Execute(remove);
    remove.WaitTillFinished();
}

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
    Task task([this]
    {
        glLinkProgram(program);
        printGlError("Compile error");
        offsetLoc = glGetUniformLocation(program, "offset");
        printGlError("Uniform location error");
        perspectiveLoc = glGetUniformLocation(program, "perspective");
        rotationLoc = glGetUniformLocation(program, "rotation");
        lightLoc = glGetUniformLocation(program, "light");
        intensityLoc = glGetUniformLocation(program, "intensity");
    });
    context.Execute(task);
    task.WaitTillFinished();
}

void GlProgram::Use()
{
    glUseProgram(program);
    //printGlError("Use error");
    glUniform2fv(offsetLoc, 1, glm::value_ptr(offset));
    //printGlError("Uniform error");
    glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, glm::value_ptr(perspective));
    //printGlError("Uniform error");
    glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(rotation));
    glUniform4fv(lightLoc, 1, glm::value_ptr(light));
    glUniform1f(intensityLoc, intensity);
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
            return glGetUniformLocation(program, name.c_str());
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
