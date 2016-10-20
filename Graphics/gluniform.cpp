#include "gluniform.h"

InvokationResult setFloatVectorUniform(GLuint program, GLuint location, short vectorSize, GLfloat* values, unsigned count);
InvokationResult setIntVectorUniform(GLuint program, GLuint location, short vectorSize, GLint* values, unsigned count);
InvokationResult setUnsignedVectorUniform(GLuint program, GLuint location, short vectorSize, GLuint* values, unsigned count);
InvokationResult setDoubleVectorUniform(GLuint program, GLuint location, short vectorSize, GLdouble* values, unsigned count);
InvokationResult setFloatMatrixUniform(GLuint program, GLuint location, short horizontalSize, short verticalSize, GLfloat* values, unsigned count);
InvokationResult setDoubleMatrixUniform(GLuint program, GLuint location, short horizontalSize, short verticalSize, GLdouble* values, unsigned count);

InvokationResult setUniform(GLuint program, GlUniform& uniform)
{
    if (uniform.verticalSize < 1 || uniform.verticalSize < 1)
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
    switch(vectorSize)
    {
    case 1:
        glProgramUniform1fvEXT(program, location, count, values);
        break;
    case 2:
        glProgramUniform2fvEXT(program, location, count, values);
        break;
    case 3:
        glProgramUniform3fvEXT(program, location, count, values);
        break;
    case 4:
        glProgramUniform4fvEXT(program, location, count, values);
        break;
    default:
        return InvokationResult::ERROR;
    }
    return InvokationResult::SUCCESS;
}

InvokationResult setIntVectorUniform(GLuint program, GLuint location, short vectorSize, GLint* values, unsigned count)
{
    switch(vectorSize)
    {
    case 1:
        glProgramUniform1ivEXT(program, location, count, values);
        break;
    case 2:
        glProgramUniform2ivEXT(program, location, count, values);
        break;
    case 3:
        glProgramUniform3ivEXT(program, location, count, values);
        break;
    case 4:
        glProgramUniform4ivEXT(program, location, count, values);
        break;
    default:
        return InvokationResult::ERROR;
    }
    return InvokationResult::SUCCESS;
}

InvokationResult setUnsignedVectorUniform(GLuint program, GLuint location, short vectorSize, GLuint* values, unsigned count)
{
    switch(vectorSize)
    {
    case 1:
        glProgramUniform1uivEXT(program, location, count, values);
        break;
    case 2:
        glProgramUniform2uivEXT(program, location, count, values);
        break;
    case 3:
        glProgramUniform3uivEXT(program, location, count, values);
        break;
    case 4:
        glProgramUniform4uivEXT(program, location, count, values);
        break;
    default:
        return InvokationResult::ERROR;
    }
    return InvokationResult::SUCCESS;
}

InvokationResult setDoubleVectorUniform(GLuint program, GLuint location, short vectorSize, GLdouble* values, unsigned count)
{
    switch(vectorSize)
    {
    case 1:
        glProgramUniform1dv(program, location, count, values);
        break;
    case 2:
        glProgramUniform2dv(program, location, count, values);
        break;
    case 3:
        glProgramUniform3dv(program, location, count, values);
        break;
    case 4:
        glProgramUniform4dv(program, location, count, values);
        break;
    default:
        return InvokationResult::ERROR;
    }
    return InvokationResult::SUCCESS;
}

InvokationResult setFloatMatrixUniform(GLuint program, GLuint location, short horizontalSize, short verticalSize, GLfloat* values, unsigned count)
{
    switch(horizontalSize)
    {
    case 2:
        switch(verticalSize)
        {
        case 2:
            glProgramUniformMatrix2fvEXT(program, location, count, GL_FALSE, values);
            break;
        case 3:
            glProgramUniformMatrix2x3fvEXT(program, location, count, GL_FALSE, values);
            break;
        case 4:
            glProgramUniformMatrix2x4fvEXT(program, location, count, GL_FALSE, values);
            break;
        }
        break;
    case 3:
        switch(verticalSize)
        {
        case 2:
            glProgramUniformMatrix3x2fvEXT(program, location, count, GL_FALSE, values);
            break;
        case 3:
            glProgramUniformMatrix3fvEXT(program, location, count, GL_FALSE, values);
            break;
        case 4:
            glProgramUniformMatrix3x4fvEXT(program, location, count, GL_FALSE, values);
            break;
        }
        break;
    case 4:
        switch(verticalSize)
        {
        case 2:
            glProgramUniformMatrix4x2fvEXT(program, location, count, GL_FALSE, values);
            break;
        case 3:
            glProgramUniformMatrix4x3fvEXT(program, location, count, GL_FALSE, values);
            break;
        case 4:
            glProgramUniformMatrix4fvEXT(program, location, count, GL_FALSE, values);
            break;
        }
        break;
    }
    return InvokationResult::SUCCESS;
}

InvokationResult setDoubleMatrixUniform(GLuint program, GLuint location, short horizontalSize, short verticalSize, GLdouble* values, unsigned count)
{
    switch(horizontalSize)
    {
    case 2:
        switch(verticalSize)
        {
        case 2:
            glProgramUniformMatrix2dv(program, location, count, GL_FALSE, values);
            break;
        case 3:
            glProgramUniformMatrix2x3dv(program, location, count, GL_FALSE, values);
            break;
        case 4:
            glProgramUniformMatrix2x4dv(program, location, count, GL_FALSE, values);
            break;
        }
        break;
    case 3:
        switch(verticalSize)
        {
        case 2:
            glProgramUniformMatrix3x2dv(program, location, count, GL_FALSE, values);
            break;
        case 3:
            glProgramUniformMatrix3dv(program, location, count, GL_FALSE, values);
            break;
        case 4:
            glProgramUniformMatrix3x4dv(program, location, count, GL_FALSE, values);
            break;
        }
        break;
    case 4:
        switch(verticalSize)
        {
        case 2:
            glProgramUniformMatrix4x2dv(program, location, count, GL_FALSE, values);
            break;
        case 3:
            glProgramUniformMatrix4x3dv(program, location, count, GL_FALSE, values);
            break;
        case 4:
            glProgramUniformMatrix4dv(program, location, count, GL_FALSE, values);
            break;
        }
        break;
    }
    return InvokationResult::SUCCESS;
}
