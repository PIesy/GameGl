//
// Created by akann on 10/2/17.
//

#ifndef GAMEGL_GL_PROGRAM_HANDLE_H
#define GAMEGL_GL_PROGRAM_HANDLE_H


#include <type_traits>
#include <string>
#include "../../../Graphics/graphicsclasses.h"
#include "Wrappers/glprogram.h"

struct GlUniform: public UniformValue
{
    GLuint location = 0;
};

class GlProgramHandle : public Shader
{
    size_t handle;
    ShaderType type;
    std::unordered_map<std::string, GlUniform> uniforms;
    void setUniform(GlUniform& uniform);
public:
    GlProgramHandle(const std::string& source, ShaderType type);
    GlProgramHandle(const GlProgramHandle& source) = default;
    GlProgramHandle(GlProgramHandle&& source) = default;
    const gl::GlProgram& GetProgram() const;
    size_t GetHandle() const;
    void SetUniform(const std::string& name, const UniformValue& value) override;

    ShaderType GetType() const override;
};


#endif //GAMEGL_GL_PROGRAM_HANDLE_H
