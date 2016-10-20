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
    Vec2 offset;
    Mat4 perspective;
    Mat4 rotation;
    Vec4 light;
    float intensity;
    GLuint offsetLoc;
    GLuint perspectiveLoc;
    GLuint rotationLoc;
    GLuint lightLoc;
    GLuint intensityLoc;
public:
    ~GlProgram();
    GlProgram(RenderingContext& context);
    void Attach(const Shader& shader);
    void Detach(const Shader& shader);
    void Compile();
    void Use();
    void SetOffset(Vec2 offset);
    void SetPerspective(Mat4 perspective);
    void SetRotation(Mat4 rotation);
    void SetLight(Vec4 light);
    void SetIntensity(float intensity);
    InvokationResult setUniform(const std::string& name, const UniformValue& value);
    operator GLuint() const;
};

#endif // GLPROGRAM_H

