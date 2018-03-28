//
// Created by akann on 10/2/17.
//

#include "gl_program_handle.h"
#include "graphics_context_data.h"
#include "../../../Logger/logger.h"

const auto logger = Logger::GetLogger(getClassName<GlProgramHandle>());

gl::GlShaderType convertShaderType(ShaderType type);

GlProgramHandle::GlProgramHandle(const std::string& source, ShaderType type) : type(type)
{
    handle = std::hash<std::string>{}(source);
    if (!sharedGlData->programMap.Contains(handle))
        sharedGlData->programMap.Put(handle, {convertShaderType(type), source});
}

ShaderType GlProgramHandle::GetType() const
{
    return type;
}

const gl::GlProgram& GlProgramHandle::GetProgram() const
{
    return sharedGlData->programMap.Get(handle);
}

size_t GlProgramHandle::GetHandle() const
{
    return handle;
}

void GlProgramHandle::SetUniform(const std::string& name, const UniformValue& value)
{
    GlUniform uniform;

    if (uniforms.find(name) == uniforms.end())
    {
        GLint loc = sharedGlData->programMap.GetWrite(handle).GetObject().GetUniformLocation(name);
        if (loc < 0)
            return;
        uniform.count = value.count;
        uniform.horizontalSize = value.horizontalSize;
        uniform.type = value.type;
        uniform.verticalSize = value.verticalSize;
        uniform.location = static_cast<GLuint>(loc);
        uniforms.insert({name, uniform});
    }
    else
        uniform = uniforms[name];
    uniform.value = value.value;
    setUniform(uniform);
}

void GlProgramHandle::setUniform(GlUniform& uniform)
{
    if ((uniform.horizontalSize < 1 || uniform.verticalSize < 1) || (uniform.horizontalSize > 4 || uniform.verticalSize > 4))
    {
        logger.LogWarning("Unsupported uniform size: " + std::to_string(uniform.verticalSize) + "x" + std::to_string(uniform.horizontalSize));
        return;
    }
    if (uniform.verticalSize == 1)
    {
        switch(uniform.type)
        {
            case UniformTypes::FLOAT:
                sharedGlData->programMap.GetWrite(handle).GetObject()
                        .SetFloatVectorUniform(uniform.location, uniform.horizontalSize, (GLfloat*)uniform.value, uniform.count);
                break;
            case UniformTypes::INTEGER:
                sharedGlData->programMap.GetWrite(handle).GetObject()
                        .SetIntVectorUniform(uniform.location, uniform.horizontalSize, (GLint*)uniform.value, uniform.count);
                break;
            case UniformTypes::UNSIGNED:
                sharedGlData->programMap.GetWrite(handle).GetObject()
                        .SetUnsignedVectorUniform(uniform.location, uniform.horizontalSize, (GLuint*)uniform.value, uniform.count);
                break;
            case UniformTypes::DOUBLE:
                sharedGlData->programMap.GetWrite(handle).GetObject()
                        .SetDoubleVectorUniform(uniform.location, uniform.horizontalSize, (GLdouble*)uniform.value, uniform.count);
                break;
        }
    }
    else
    {
        switch(uniform.type)
        {
            case UniformTypes::FLOAT:
                sharedGlData->programMap.GetWrite(handle).GetObject()
                        .SetFloatMatrixUniform(uniform.location, uniform.horizontalSize, uniform.verticalSize, (GLfloat*)uniform.value, uniform.count);
                break;
            case UniformTypes::DOUBLE:
                sharedGlData->programMap.GetWrite(handle).GetObject()
                        .SetDoubleMatrixUniform(uniform.location, uniform.horizontalSize, uniform.verticalSize, (GLdouble*)uniform.value, uniform.count);
                break;
            default:
                logger.LogWarning("Not supported matrix uniform type");
        }
    }
}

gl::GlShaderType convertShaderType(ShaderType type)
{
    switch (type)
    {
        case ShaderType::VertexShader:
            return gl::GlShaderType::Vertex;
        case ShaderType::FragmentShader:
            return gl::GlShaderType::Fragment;
        case ShaderType::GeometryShader:
            return gl::GlShaderType::Geometry;
        default:
            return gl::GlShaderType::Undefined;
    }
}