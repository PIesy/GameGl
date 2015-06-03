#ifndef GRAPHICSCLASSES_H
#define GRAPHICSCLASSES_H

#include "Helpers/invokable.h"
#include "Helpers/size.h"
#include "Core/service.h"
#include "Math/mathconstants.h"

enum class ShaderType { VertexShader, FragmentShader };

using WindowSize = Size;

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
    virtual ShaderType getType() const = 0;
};

class Program
{
public:
    virtual ~Program(){}
    virtual void Attach(const Shader& shader) = 0;
    virtual void Detach(const Shader& shader) = 0;
    virtual void Compile() = 0;
    virtual void Use() = 0;
    virtual void SetOffset(Vec2 offset) = 0;
    virtual void SetPerspective(Mat4 perspective) = 0;
    virtual void SetRotation(Mat4 rotation) = 0;
    virtual void SetLight(Vec4 light) = 0;
    virtual void SetIntensity(float intensity) = 0;
};

struct RGBA_Color
{
    Vec4 color;
    RGBA_Color(const Vec4 init):color(init) {}
    RGBA_Color() {}
};

struct Vertex
{
    Vec3 coords = {0,0,0};
    Vec3 normal = {0,0,0};
    Vec4 color = {1,0,0,1};
};

struct RenderObject
{
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    Program* program = nullptr;
};


class VertexObject
{
    RenderObject object;
public:
    VertexObject(){}
    VertexObject(RenderObject& base);
    VertexObject(RenderObject&& base);
    VertexObject(VertexObject&& src);
    VertexObject(const VertexObject& src);
    operator RenderObject&();
    VertexObject& operator =(const VertexObject& src);
    VertexObject& operator =(const RenderObject& src);
    RenderObject& data();
    void Append(const VertexObject& src, float offset_x = 0, float offset_y = 0);
    void Append(const RenderObject& src, float offset_x = 0, float offset_y = 0);
};

struct Scene
{
    std::vector<VertexObject*> objects;
    short passes = 0;
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

class Renderer: public Service
{
public:
    virtual void SetWindow(const Window& window) = 0;
    virtual void Draw(const Scene& scene) = 0;
    virtual void SetViewport(int width, int height) = 0;
};
#endif // GRAPHICSCLASSES_H

