//
// Created by akann on 10/6/17.
//

#ifndef GAMEGL_GL_PROGRAM_PROXY_H
#define GAMEGL_GL_PROGRAM_PROXY_H

#include "../../../Graphics/graphicsclasses.h"
#include "gl_program_handle.h"
#include "sdlclasses.h"

class GlProgramProxy : public Shader
{
    GlProgramHandle handle;
    SdlGLContext& context;
public:
    GlProgramProxy(GlProgramHandle handle, SdlGLContext& context);
    ShaderType GetType() const override;
    const gl::GlProgram& GetProgram() const;
    size_t GetHandle() const;
    void SetUniform(const std::string& name, const UniformValue& value) override;
};


#endif //GAMEGL_GL_PROGRAM_PROXY_H
