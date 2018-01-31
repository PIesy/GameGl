//
// Created by akann on 9/7/17.
//

#include "glprogram.h"
#include "../../../../Logger/logger.h"
#include "../../../../Helpers/helpers.h"

const auto logger = Logger::GetLogger(getClassName<gl::GlProgram>());

gl::GlProgram::GlProgram()
{
    program = gl::program::create();
}

gl::GlProgram::GlProgram(gl::GlShaderType type, const std::string& source) : type(type)
{
    program = gl::program::create(integral(type), source);
}

gl::GlProgram::GlProgram(gl::GlProgram&& src) noexcept : type(src.type)
{
    program = src.program;
    src.program = 0;
}

gl::GlProgram::~GlProgram()
{
    gl::program::erase(program);
}

void gl::GlProgram::AttachShader(gl::GlShader& shader)
{
    gl::program::attachShader(program, shader.GetId());
}

void gl::GlProgram::DetachShader(gl::GlShader& shader)
{
    gl::program::detachShader(program, shader.GetId());
}

void gl::GlProgram::Link()
{
    gl::program::link(program);
}

void gl::GlProgram::Use()
{
    gl::program::use(program);
}

void gl::GlProgram::SetFloatVectorUniform(GLuint location, short vectorSize, GLfloat* values, unsigned count)
{
    switch (vectorSize)
    {
        case 1:
            gl::program::setUniform1f(program, location, count, values);
            break;
        case 2:
            gl::program::setUniform2f(program, location, count, values);
            break;
        case 3:
            gl::program::setUniform3f(program, location, count, values);
            break;
        case 4:
            gl::program::setUniform4f(program, location, count, values);
            break;
        default:
            logger.LogWarning("Unsupported uniform; type: float, size: " + std::to_string(vectorSize));
            return;
    }
}

void gl::GlProgram::SetIntVectorUniform(GLuint location, short vectorSize, GLint* values, unsigned count)
{
    switch (vectorSize)
    {
        case 1:
            gl::program::setUniform1i(program, location, count, values);
            break;
        case 2:
            gl::program::setUniform2i(program, location, count, values);
            break;
        case 3:
            gl::program::setUniform3i(program, location, count, values);
            break;
        case 4:
            gl::program::setUniform4i(program, location, count, values);
            break;
        default:
            logger.LogWarning("Unsupported uniform; type: int, size: " + std::to_string(vectorSize));
            return;
    }
}

void gl::GlProgram::SetUnsignedVectorUniform(GLuint location, short vectorSize, GLuint* values, unsigned count)
{
    switch (vectorSize)
    {
        case 1:
            gl::program::setUniform1ui(program, location, count, values);
            break;
        case 2:
            gl::program::setUniform2ui(program, location, count, values);
            break;
        case 3:
            gl::program::setUniform3ui(program, location, count, values);
            break;
        case 4:
            gl::program::setUniform4ui(program, location, count, values);
            break;
        default:
            logger.LogWarning("Unsupported uniform; type: unsigned, size: " + std::to_string(vectorSize));
            return;
    }
}

void gl::GlProgram::SetDoubleVectorUniform(GLuint location, short vectorSize, GLdouble* values, unsigned count)
{
    switch (vectorSize)
    {
        case 1:
            gl::program::setUniform1d(program, location, count, values);
            break;
        case 2:
            gl::program::setUniform2d(program, location, count, values);
            break;
        case 3:
            gl::program::setUniform3d(program, location, count, values);
            break;
        case 4:
            gl::program::setUniform4d(program, location, count, values);
            break;
        default:
            logger.LogWarning("Unsupported uniform; type: double, size: " + std::to_string(vectorSize));
            return;
    }
}

void gl::GlProgram::SetFloatMatrixUniform(GLuint location, short horizontalSize, short verticalSize, GLfloat* values, unsigned count)
{
    switch (horizontalSize)
    {
        case 2:
            switch (verticalSize)
            {
                case 2:
                    gl::program::setUniform2x2f(program, location, count, values);
                    break;
                case 3:
                    glUniformMatrix2x3fv(location, count, GL_FALSE, values);
                    break;
                case 4:
                    glUniformMatrix2x4fv(location, count, GL_FALSE, values);
                    break;
                default:
                    logger.LogWarning("Unsupported uniform; type: float, size: " + std::to_string(horizontalSize) + "x" + std::to_string(verticalSize));
                    break;
            }
            break;
        case 3:
            switch (verticalSize)
            {
                case 2:
                    glUniformMatrix3x2fv(location, count, GL_FALSE, values);
                    break;
                case 3:
                    gl::program::setUniform3x3f(program, location, count, values);
                    break;
                case 4:
                    glUniformMatrix3x4fv(location, count, GL_FALSE, values);
                    break;
                default:
                    logger.LogWarning("Unsupported uniform; type: float, size: " + std::to_string(horizontalSize) + "x" + std::to_string(verticalSize));
                    break;
            }
            break;
        case 4:
            switch (verticalSize)
            {
                case 2:
                    glUniformMatrix4x2fv(location, count, GL_FALSE, values);
                    break;
                case 3:
                    glUniformMatrix4x3fv(location, count, GL_FALSE, values);
                    break;
                case 4:
                    gl::program::setUniform4x4f(program, location, count, values);
                    break;
                default:
                    logger.LogWarning("Unsupported uniform; type: float, size: " + std::to_string(horizontalSize) + "x" + std::to_string(verticalSize));
                    break;
            }
            break;
        default:
            logger.LogWarning("Unsupported uniform; type: float, size: " + std::to_string(horizontalSize) + "x" + std::to_string(verticalSize));
            break;
    }
}

void gl::GlProgram::SetDoubleMatrixUniform(GLuint location, short horizontalSize, short verticalSize, GLdouble* values, unsigned count)
{
    switch (horizontalSize)
    {
        case 2:
            switch (verticalSize)
            {
                case 2:
                    glUniformMatrix2dv(location, count, GL_FALSE, values);
                    break;
                case 3:
                    glUniformMatrix2x3dv(location, count, GL_FALSE, values);
                    break;
                case 4:
                    glUniformMatrix2x4dv(location, count, GL_FALSE, values);
                    break;
                default:
                    logger.LogWarning("Unsupported uniform; type: double, size: " + std::to_string(horizontalSize) + "x" + std::to_string(verticalSize));
                    break;
            }
            break;
        case 3:
            switch (verticalSize)
            {
                case 2:
                    glUniformMatrix3x2dv(location, count, GL_FALSE, values);
                    break;
                case 3:
                    glUniformMatrix3dv(location, count, GL_FALSE, values);
                    break;
                case 4:
                    glUniformMatrix3x4dv(location, count, GL_FALSE, values);
                    break;
                default:
                    logger.LogWarning("Unsupported uniform; type: double, size: " + std::to_string(horizontalSize) + "x" + std::to_string(verticalSize));
                    break;
            }
            break;
        case 4:
            switch (verticalSize)
            {
                case 2:
                    glUniformMatrix4x2dv(location, count, GL_FALSE, values);
                    break;
                case 3:
                    glUniformMatrix4x3dv(location, count, GL_FALSE, values);
                    break;
                case 4:
                    glUniformMatrix4dv(location, count, GL_FALSE, values);
                    break;
                default:
                    logger.LogWarning("Unsupported uniform; type: double, size: " + std::to_string(horizontalSize) + "x" + std::to_string(verticalSize));
                    break;
            }
            break;
        default:
            logger.LogWarning("Unsupported uniform; type: double, size: " + std::to_string(horizontalSize) + "x" + std::to_string(verticalSize));
            break;
    }
}

gl::GlShaderType gl::GlProgram::GetType() const
{
    return type;
}

GLint gl::GlProgram::GetUniformLocation(const std::string& name)
{
    return gl::program::getUniformLocation(program, name);
}

GLuint gl::GlProgram::GetId() const
{
    return program;
}
