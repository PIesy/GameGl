#include "graphicsapi.h"
#include <cstring>

const std::string EMPTY_ATTRIBUTE = "<none>";

GraphicsObject::GraphicsObject(RenderData& base)
{
    object = base;
}

GraphicsObject::GraphicsObject(RenderData&& base)
{
    object = base;
}

GraphicsObject::GraphicsObject(GraphicsObject&& src)
{
    object = src.object;
    attributes = src.attributes;
}

GraphicsObject::GraphicsObject(const GraphicsObject &src)
{
    object = src.object;
    attributes = src.attributes;
}

GraphicsObject::operator RenderData&()
{
    return object;
}

GraphicsObject& GraphicsObject::operator=(const GraphicsObject& src)
{
    object = src.object;
    attributes = src.attributes;
    return *this;
}

GraphicsObject& GraphicsObject::operator=(const RenderData& src)
{
    object = src;
    return *this;
}

RenderData& GraphicsObject::data()
{
    return object;
}

void GraphicsObject::Append(const GraphicsObject& src, float offset_x, float offset_y)
{
    Append(src.object, offset_x, offset_y);
}

void GraphicsObject::Append(const RenderData &src, float offset_x, float offset_y)
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

void GraphicsObject::Scale(float x, float y, float z)
{
    for (Vertex& vertex : object.vertices)
    {
        vertex.coords.x *= x;
        vertex.coords.y *= y;
        vertex.coords.z *= z;
    }
}

void GraphicsObject::Move(float x, float y, float z)
{
    for (Vertex& vertex : object.vertices)
    {
        vertex.coords.x += x;
        vertex.coords.y += y;
        vertex.coords.z += z;
    }
}

void GraphicsObject::setAttribute(const std::string& attributeName, const std::string& attributeValue)
{
    if (attributes.count(attributeName))
    {
        attributes.at(attributeName) = attributeValue;
    }
    else
    {
        attributes.insert({attributeName, attributeValue});
    }
}

const std::string& GraphicsObject::getAttribute(const std::string& attributeName) const
{
    if (attributes.count(attributeName))
    {
        return attributes.at(attributeName);
    }
    return EMPTY_ATTRIBUTE;
}
