#include "gluniform.h"

InvokationResult setFloatVectorUniform(GLuint program, GLuint location, short vectorSize, GLfloat* values, unsigned count);
InvokationResult setIntVectorUniform(GLuint program, GLuint location, short vectorSize, GLint* values, unsigned count);
InvokationResult setUnsignedVectorUniform(GLuint program, GLuint location, short vectorSize, GLuint* values, unsigned count);
InvokationResult setDoubleVectorUniform(GLuint program, GLuint location, short vectorSize, GLdouble* values, unsigned count);
InvokationResult setFloatMatrixUniform(GLuint program, GLuint location, short horizontalSize, short verticalSize, GLfloat* values, unsigned count);
InvokationResult setDoubleMatrixUniform(GLuint program, GLuint location, short horizontalSize, short verticalSize, GLdouble* values, unsigned count);

InvokationResult setUniform(GLuint program, GlUniform& uniform)
{
    if (uniform.horizontalSize < 1 || uniform.verticalSize < 1)
        return InvokationResult::ERROR;
    if (uniform.horizontalSize > 4 || uniform.verticalSize > 4)
        return InvokationResult::ERROR;
    if (uniform.verticalSize == 1)
    {
        switch(uniform.type)
        {
        case UniformTypes::FLOAT:
            setFloatVectorUniform(program, uniform.location, uniform.horizontalSize, (GLfloat*)uniform.value, uniform.count);
            break;
        case UniformTypes::INTEGER:
            setIntVectorUniform(program, uniform.location, uniform.horizontalSize, (GLint*)uniform.value, uniform.count);
            break;
        case UniformTypes::UNSIGNED:
            setUnsignedVectorUniform(program, uniform.location, uniform.horizontalSize, (GLuint*)uniform.value, uniform.count);
            break;
        case UniformTypes::DOUBLE:
            setDoubleVectorUniform(program, uniform.location, uniform.horizontalSize, (GLdouble*)uniform.value, uniform.count);
            break;
        }
    }
    else
    {
        switch(uniform.type)
        {
        case UniformTypes::FLOAT:
            setFloatMatrixUniform(program, uniform.location, uniform.horizontalSize, uniform.verticalSize, (GLfloat*)uniform.value, uniform.count);
            break;
        case UniformTypes::DOUBLE:
            setDoubleMatrixUniform(program, uniform.location, uniform.horizontalSize, uniform.verticalSize, (GLdouble*)uniform.value, uniform.count);
            break;
        default:
            return InvokationResult::NOT_SUPPORTED;
        }
    }
    return InvokationResult::SUCCESS;
}

InvokationResult setFloatVectorUniform(GLuint program, GLuint location, short vectorSize, GLfloat* values, unsigned count)
{
    glUseProgram(program);
    switch(vectorSize)
    {
    case 1:
        gl::program::setUniform1f(location, count, values);
        break;
    case 2:
        gl::program::setUniform2f(location, count, values);
        break;
    case 3:
        gl::program::setUniform3f(location, count, values);
        break;
    case 4:
        gl::program::setUniform4f(location, count, values);
        break;
    default:
        return InvokationResult::ERROR;
    }
    glUseProgram(0);
    return InvokationResult::SUCCESS;
}

InvokationResult setIntVectorUniform(GLuint program, GLuint location, short vectorSize, GLint* values, unsigned count)
{
    glUseProgram(program);
    switch(vectorSize)
    {
    case 1:
        gl::program::setUniform1i(location, count, values);
        break;
    case 2:
        gl::program::setUniform2i(location, count, values);
        break;
    case 3:
        gl::program::setUniform3i(location, count, values);
        break;
    case 4:
        gl::program::setUniform4i(location, count, values);
        break;
    default:
        return InvokationResult::ERROR;
    }
    glUseProgram(0);
    return InvokationResult::SUCCESS;
}

InvokationResult setUnsignedVectorUniform(GLuint program, GLuint location, short vectorSize, GLuint* values, unsigned count)
{
    glUseProgram(program);
    switch(vectorSize)
    {
    case 1:
        gl::program::setUniform1ui(location, count, values);
        break;
    case 2:
        gl::program::setUniform2ui(location, count, values);
        break;
    case 3:
        gl::program::setUniform3ui(location, count, values);
        break;
    case 4:
        gl::program::setUniform4ui(location, count, values);
        break;
    default:
        return InvokationResult::ERROR;
    }
    glUseProgram(0);
    return InvokationResult::SUCCESS;
}

InvokationResult setDoubleVectorUniform(GLuint program, GLuint location, short vectorSize, GLdouble* values, unsigned count)
{
    glUseProgram(program);
    switch(vectorSize)
    {
    case 1:
        glUniform1dv(location, count, values);
        break;
    case 2:
        glUniform2dv(location, count, values);
        break;
    case 3:
        glUniform3dv(location, count, values);
        break;
    case 4:
        glUniform4dv(location, count, values);
        break;
    default:
        return InvokationResult::ERROR;
    }
    glUseProgram(0);
    return InvokationResult::SUCCESS;
}

InvokationResult setFloatMatrixUniform(GLuint program, GLuint location, short horizontalSize, short verticalSize, GLfloat* values, unsigned count)
{
    glUseProgram(program);
    switch(horizontalSize)
    {
    case 2:
        switch(verticalSize)
        {
        case 2:
            gl::program::setUniform2x2f(location, count, values);
            break;
        case 3:
            glUniformMatrix2x3fv(location, count, GL_FALSE, values);
            break;
        case 4:
            glUniformMatrix2x4fv(location, count, GL_FALSE, values);
            break;
        }
        break;
    case 3:
        switch(verticalSize)
        {
        case 2:
            glUniformMatrix3x2fv(location, count, GL_FALSE, values);
            break;
        case 3:
            gl::program::setUniform3x3f(location, count, values);
            break;
        case 4:
            glUniformMatrix3x4fv(location, count, GL_FALSE, values);
            break;
        }
        break;
    case 4:
        switch(verticalSize)
        {
        case 2:
            glUniformMatrix4x2fv(location, count, GL_FALSE, values);
            break;
        case 3:
            glUniformMatrix4x3fv(location, count, GL_FALSE, values);
            break;
        case 4:
            gl::program::setUniform4x4f(location, count, values);
            break;
        }
        break;
    }
    glUseProgram(0);
    return InvokationResult::SUCCESS;
}

InvokationResult setDoubleMatrixUniform(GLuint program, GLuint location, short horizontalSize, short verticalSize, GLdouble* values, unsigned count)
{
    glUseProgram(program);
    switch(horizontalSize)
    {
    case 2:
        switch(verticalSize)
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
        }
        break;
    case 3:
        switch(verticalSize)
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
        }
        break;
    case 4:
        switch(verticalSize)
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
        }
        break;
    }
    glUseProgram(0);
    return InvokationResult::SUCCESS;
}
