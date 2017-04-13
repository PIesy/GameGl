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
public:
    ~GlProgram();
    GlProgram(RenderingContext& context);
    void Attach(const Shader& shader);
    void Detach(const Shader& shader);
    void Compile();
    void Use();
    InvokationResult setUniform(const std::string& name, const UniformValue& value);
    operator GLuint() const;
};

#endif // GLPROGRAM_H

