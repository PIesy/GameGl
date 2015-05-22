#ifndef GRAPHICSCLASSES_H
#define GRAPHICSCLASSES_H

#include "Helpers/invokable.h"
#include "Helpers/size.h"
#include "Core/service.h"

enum class ShaderType { VertexShader, FragmentShader };

using WindowSize = Size;

class Window
{
public:
    virtual ~Window() {}
    virtual void Close() = 0;
    virtual WindowSize getSize() = 0;
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
};

struct RGBA_Color
{
    float color[4];
};

struct Vertex
{
    float coords[4] = {0,0,0,1};
    float color[4] = {1,1,1,1};
};

struct Indices
{
    unsigned int* indices = nullptr;
    unsigned int count = 0;
};

struct RenderObject
{
    Vertex* vertices = nullptr;
    unsigned int vertexCount = 0;
    Indices indices;
    short dimensions = 2;
    Program* program = nullptr;
};


class VertexObject
{
    RenderObject object;
    void copyObject(RenderObject* object, const RenderObject *src);
    void clear();
public:
    VertexObject(){}
    VertexObject(RenderObject* base);
    VertexObject(RenderObject base);
    VertexObject(VertexObject&& src);
    ~VertexObject();
    VertexObject(const VertexObject& src);
    operator RenderObject&();
    VertexObject& operator =(const VertexObject& src);
    VertexObject& operator =(const RenderObject& src);
    RenderObject* data();
    void Append(const VertexObject& src, float offset_x = 0, float offset_y = 0);
    void Append(const RenderObject& src, float offset_x = 0, float offset_y = 0);
};

struct Scene
{
    VertexObject** objects;
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
    virtual Window& getWindow() = 0;
};

class Renderer: public Service
{
public:
    virtual void SetWindow(const Window& window) = 0;
    virtual void Draw(const Scene& scene) = 0;
    virtual void SetViewport(int width, int height) = 0;
};
#endif // GRAPHICSCLASSES_H

