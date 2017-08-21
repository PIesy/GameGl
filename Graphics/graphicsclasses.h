#ifndef GRAPHICSCLASSES_H
#define GRAPHICSCLASSES_H

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include "../Core/invokable.h"
#include "../Helpers/size.h"
#include "../Helpers/invokationresult.h"
#include "../Core/service/service.h"
#include "../Math/mathconstants.h"
#include "uniform.h"

enum class ShaderType { VertexShader, FragmentShader, GeometryShader };
class Program;

using WindowSize = Size;
using ConfigFunction = std::function<void(Program&)>;
const ConfigFunction defaultConfig = [](Program&) {};

class BasicWindow
{
public:
    virtual ~BasicWindow() {}
    virtual void Close() = 0;
    virtual WindowSize getSize() = 0;
};

class Window: public BasicWindow
{
    BasicWindow& window;
public:
    Window(BasicWindow& window):window(window) {}
    void Close() { window.Close(); }
    WindowSize getSize() { return window.getSize(); }
    BasicWindow& getWindow() const { return window; }
};

class Shader
{
public:
    virtual ~Shader(){}
    virtual ShaderType GetType() const = 0;
};

class Program
{
public:
    virtual ~Program(){}
    virtual void Attach(const Shader& shader) = 0;
    virtual void Detach(const Shader& shader) = 0;
    virtual void Compile() = 0;
    virtual void Use() = 0;
    virtual InvokationResult SetUniform(const std::string& name, const UniformValue& value) = 0;

    template<typename T>
    InvokationResult SetUniform(const T& value, const std::string& name, int count = 1);
};

template<>
InvokationResult Program::SetUniform<Vec4>(const Vec4& value, const std::string& name, int count);
template<>
InvokationResult Program::SetUniform<Vec3>(const Vec3& value, const std::string& name, int count);
template<>
InvokationResult Program::SetUniform<Vec2>(const Vec2& value, const std::string& name, int count);
template<>
InvokationResult Program::SetUniform<Mat4>(const Mat4& value, const std::string& name, int count);
template<>
InvokationResult Program::SetUniform<Mat3>(const Mat3& value, const std::string& name, int count);
template<>
InvokationResult Program::SetUniform<float>(const float& value, const std::string& name, int count);
template<>
InvokationResult Program::SetUniform<int>(const int& value, const std::string& name, int count);
template<>
InvokationResult Program::SetUniform<bool>(const bool& value, const std::string& name, int count);

struct RGBA_Color
{
    Vec4 color;
    RGBA_Color(const Vec4 init):color(init) {}
    RGBA_Color() {}
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
    virtual ~RenderingContext() {}
    virtual void Execute(const Invokable& invokable) = 0;
    virtual void Destroy() = 0;
    virtual void SetWindow(const Window& window) = 0;
    virtual void MakeCurrent() = 0;
    virtual Window getWindow() = 0;
};
#endif // GRAPHICSCLASSES_H

