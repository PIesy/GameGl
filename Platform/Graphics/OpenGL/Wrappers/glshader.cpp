//
// Created by akann on 9/7/17.
//

#include "glshader.h"
#include "../../../../Helpers/helpers.h"

gl::GlShader::GlShader(gl::GlShaderType type)
{
    shader = gl::shader::create(integral(type));
}

gl::GlShader::GlShader(gl::GlShader&& rhs) noexcept
{
    this->shader = rhs.shader;
    rhs.shader = 0;
}

gl::GlShader::~GlShader()
{
    gl::shader::erase(shader);
}

GLuint gl::GlShader::GetId() const
{
    return shader;
}

void gl::GlShader::SetSource(const std::string& source)
{
    gl::shader::setSource(shader, source);
}
