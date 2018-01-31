#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "glshader.h"
#include <unordered_map>
#include <string>
#include "gluniform.h"

class GlProgram: public Program
{
    std::unordered_map<std::string, GlUniform> uniforms;
    RenderingContext& context;
    GLuint program = 0;
    bool isValid = true;
public:
    ~GlProgram();
    explicit GlProgram(RenderingContext& context);
    GlProgram(const GlProgram&) = delete;
    GlProgram(GlProgram&& src);
    void Attach(const Shader& shader) override;
    void Detach(const Shader& shader) override;
    void Finalize() override;
    void Use() override;
    void SetUniform(const std::string& name, const UniformValue& value) override;
    operator GLuint() const;
};

#endif // GLPROGRAM_H

