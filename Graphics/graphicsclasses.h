#ifndef GRAPHICSCLASSES_H
#define GRAPHICSCLASSES_H

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include "../Core/invokable.h"
#include "../Helpers/size.h"
#include "../Core/service/service.h"
#include "../Math/mathconstants.h"
#include "uniform.h"

enum class ShaderType { VertexShader, FragmentShader, GeometryShader };
class Program;

using WindowSize = Size;
using ConfigFunction = std::function<void(Program&)>;
using ShaderConfiguration = std::function<void()>;
const ShaderConfiguration defaultConfig = []() {};

class BasicWindow
{
public:
    virtual ~BasicWindow() = default;
    virtual void Close() = 0;
    virtual WindowSize GetSize() = 0;
};

class Window: public BasicWindow
{
    BasicWindow& window;
public:
    explicit Window(BasicWindow& window) : window(window) {}
    void Close() override { window.Close(); }
    WindowSize GetSize() override { return window.GetSize(); }
    BasicWindow& getWindow() const { return window; }
};

class Shader
{
public:
    virtual ~Shader() = default;
    virtual ShaderType GetType() const = 0;
    virtual void SetUniform(const std::string& name, const UniformValue& value) = 0;

    template<typename T>
    void SetUniform(const T& value, const std::string& name, int count = 1);
};

class Program
{
public:
    virtual ~Program() = default;
    virtual void Attach(const Shader& shader) = 0;
    virtual void Detach(const Shader& shader) = 0;
    virtual void Finalize() = 0;
    virtual void Use() = 0;
    virtual void SetUniform(const std::string& name, const UniformValue& value) = 0;

    template<typename T>
    void SetUniform(const T& value, const std::string& name, int count = 1);
};

template<>
void Shader::SetUniform<Vec4>(const Vec4& value, const std::string& name, int count);
template<>
void Shader::SetUniform<Vec3>(const Vec3& value, const std::string& name, int count);
template<>
void Shader::SetUniform<Vec2>(const Vec2& value, const std::string& name, int count);
template<>
void Shader::SetUniform<Mat4>(const Mat4& value, const std::string& name, int count);
template<>
void Shader::SetUniform<Mat3>(const Mat3& value, const std::string& name, int count);
template<>
void Shader::SetUniform<float>(const float& value, const std::string& name, int count);
template<>
void Shader::SetUniform<int>(const int& value, const std::string& name, int count);
template<>
void Shader::SetUniform<bool>(const bool& value, const std::string& name, int count);

struct RGBA_Color
{
    Vec4 color;
    RGBA_Color(const Vec4 init) : color(init) {}
    RGBA_Color() = default;
};

struct Vertex
{
    Vec3 coords = {0,0,0};
    Vec3 normal = {0,0,1};
    Vec4 color = {1,0,0,1};
    Vec2 uv = {0, 0};
    Vec3 tangent = {0, 0, 0};
};

class RenderingContext
{
public:
    virtual ~RenderingContext() = default;
    virtual void Execute(const Invokable& invokable) = 0;
    virtual void Destroy() = 0;
    virtual void SetWindow(Window& window) = 0;
    virtual void MakeCurrent() = 0;
    virtual Window GetWindow() = 0;
};
#endif // GRAPHICSCLASSES_H

