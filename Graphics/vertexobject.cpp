#include "graphicsapi.h"
#include <cstring>

VertexObject::VertexObject(RenderObject* base)
{
    copyObject(&object ,base);
}

VertexObject::VertexObject(RenderObject base)
{
    object = base;
    base.vertexCount = 0;
    base.vertices = nullptr;
    base.indices.count = 0;
    base.indices.indices = nullptr;
}

VertexObject::VertexObject(VertexObject&& src)
{
    object = src.object;
    src.object.vertexCount = 0;
    src.object.vertices = nullptr;
    src.object.indices.count = 0;
    src.object.indices.indices = nullptr;
}

void VertexObject::copyObject(RenderObject* object, const RenderObject *src)
{
    object->vertexCount = src->vertexCount;
    object->vertices = new Vertex[object->vertexCount];
    memcpy(object->vertices, src->vertices, sizeof(Vertex) * object->vertexCount);
    object->indices.count = src->indices.count;
    object->indices.indices = new unsigned int[object->indices.count];
    memcpy(object->indices.indices, src->indices.indices, sizeof(int) * object->indices.count);
    object->dimensions = src->dimensions;
    object->program = src->program;
}

void VertexObject::clear()
{
    delete object.vertices;
    delete object.indices.indices;
    object.vertexCount = 0;
    object.indices.count = 0;
}

VertexObject::VertexObject(const VertexObject &src)
{
    copyObject(&object ,&src.object);
}

VertexObject::~VertexObject()
{
    delete object.vertices;
    delete object.indices.indices;
}

VertexObject::operator RenderObject&()
{
    return object;
}

VertexObject& VertexObject::operator=(const VertexObject& src)
{
    delete object.indices.indices;
    delete object.vertices;
    copyObject(&object, &src.object);
    return *this;
}

VertexObject& VertexObject::operator=(const RenderObject& src)
{
    delete object.indices.indices;
    delete object.vertices;
    copyObject(&object, &src);
    return *this;
}

RenderObject* VertexObject::data()
{
    return &object;
}

void VertexObject::Append(const VertexObject& src, float offset_x, float offset_y)
{
    RenderObject tmp;
    unsigned int i;

    tmp.vertexCount = object.vertexCount + src.object.vertexCount;
    tmp.indices.count = object.indices.count + src.object.indices.count;
    tmp.vertices = new Vertex[tmp.vertexCount];
    memcpy(tmp.vertices, object.vertices, sizeof(Vertex) * object.vertexCount);
    tmp.indices.indices = new unsigned int[tmp.indices.count];
    memcpy(tmp.indices.indices, object.indices.indices, sizeof(int) * object.indices.count);

    for (i = 0; i < src.object.indices.count; i++)
        tmp.indices.indices[object.indices.count + i] = src.object.indices.indices[i] + object.vertexCount;
    for (i = 0; i < src.object.vertexCount; i++)
    {
        memcpy(&tmp.vertices[object.vertexCount + i], &src.object.vertices[i], sizeof(Vertex));
        tmp.vertices[object.vertexCount + i].coords[0] += offset_x;
        tmp.vertices[object.vertexCount + i].coords[1] += offset_y;
    }
    clear();
    object = tmp;
}

void VertexObject::Append(const RenderObject &src, float offset_x, float offset_y)
{
    RenderObject tmp;
    unsigned int i;

    tmp.vertexCount = object.vertexCount + src.vertexCount;
    tmp.indices.count = object.indices.count + src.indices.count;
    tmp.vertices = new Vertex[tmp.vertexCount];
    memcpy(tmp.vertices, object.vertices, sizeof(Vertex) * object.vertexCount);
    tmp.indices.indices = new unsigned int[tmp.indices.count];
    memcpy(tmp.indices.indices, object.indices.indices, sizeof(int) * object.indices.count);

    for (i = 0; i < src.indices.count; i++)
        tmp.indices.indices[object.indices.count + i] = src.indices.indices[i] + object.vertexCount;
    for (i = 0; i < src.vertexCount; i++)
    {
        memcpy(&tmp.vertices[object.vertexCount + i], &src.vertices[i], sizeof(Vertex));
        tmp.vertices[object.vertexCount + i].coords[0] += offset_x;
        tmp.vertices[object.vertexCount + i].coords[1] += offset_y;
    }
    clear();
    object = tmp;
}
