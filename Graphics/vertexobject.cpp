#include "graphicsapi.h"
#include <cstring>

VertexObject::VertexObject(RenderObject& base)
{
    object = base;
}

VertexObject::VertexObject(RenderObject&& base)
{
    object = base;
}

VertexObject::VertexObject(VertexObject&& src)
{
    object = src.object;
}

VertexObject::VertexObject(const VertexObject &src)
{
    object = src.object;
}

VertexObject::operator RenderObject&()
{
    return object;
}

VertexObject& VertexObject::operator=(const VertexObject& src)
{
    object = src.object;
    return *this;
}

VertexObject& VertexObject::operator=(const RenderObject& src)
{
    object = src;
    return *this;
}

RenderObject& VertexObject::data()
{
    return object;
}

void VertexObject::Append(const VertexObject& src, float offset_x, float offset_y)
{
    Append(src.object, offset_x, offset_y);
}

void VertexObject::Append(const RenderObject &src, float offset_x, float offset_y)
{
    int size = object.vertices.size();
    Vertex tmp;

    for (const unsigned& value: src.indices)
        object.indices.push_back(value + size);
    for (const Vertex& vertex: src.vertices)
    {
        tmp.coords[0] = vertex.coords[0] + offset_x;
        tmp.coords[1] = vertex.coords[1] + offset_y;
        object.vertices.push_back(tmp);
    }
}
