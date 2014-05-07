#ifndef GRAPHICSAPI_H
#define GRAPHICSAPI_H

#include <string>

class Shader
{
public:
    virtual ~Shader(){}
    virtual void Destroy() = 0;
    virtual operator unsigned int() = 0;
};

class Program
{
public:
    virtual ~Program(){}
    virtual void Attach(Shader*) = 0;
    virtual void Detach(Shader*) = 0;
    virtual void Compile() = 0;
    virtual void Use(void*) = 0;
    virtual operator unsigned int() = 0;
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

class GraphicsApi
{
public:
    virtual void* CreateWindow(std::string title, int x, int y) = 0;
    virtual Shader* CreateShader(std::string source, unsigned int type) = 0;
    virtual Program* CreateProgram() = 0;
    virtual void SendScene(void* scene) = 0;
    virtual void setViewport(int x, int y, int width, int height) = 0;
    virtual ~GraphicsApi(){}
};

#endif // GRAPHICSAPI_H
