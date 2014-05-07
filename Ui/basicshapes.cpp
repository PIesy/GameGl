#include "basicshapes.h"
#include <cstring>

NestedShapes Shapes::shapes;

NestedShapes::NestedShapes()
{
    initBox();
    initTriangle();
    initHexagon();
}

RenderObject NestedShapes::Box()
{
    return getCopy(box);
}

RenderObject NestedShapes::Triangle()
{
    return getCopy(triangle);
}

RenderObject NestedShapes::Hexagon()
{
    return getCopy(hexagon);
}

RenderObject NestedShapes::getCopy(const RenderObject &src)
{
    RenderObject obj;

    obj.indices.count = src.indices.count;
    obj.indices.indices = new unsigned int[obj.indices.count];
    memcpy(obj.indices.indices, src.indices.indices, sizeof(int) * obj.indices.count);
    obj.vertexCount = src.vertexCount;
    obj.vertices = new Vertex[obj.vertexCount];
    memcpy(obj.vertices, src.vertices, sizeof(Vertex) * obj.vertexCount);
    return obj;
}

void NestedShapes::initBox()
{
    int indices[6] = {0,1,2,1,3,2};
    int i, j, n = 0;

    box.vertices = new Vertex[4];
    box.indices.indices = new unsigned int[6];
    box.vertexCount = 4;
    box.indices.count = 6;
    memcpy(box.indices.indices, indices, sizeof(indices));

    for (i = -1; i < 2; i += 2)
        for (j = -1; j < 2; j += 2)
        {
            box.vertices[n].coords[0] = j;
            box.vertices[n].coords[1] = i;
            n++;
        }
}

void NestedShapes::initTriangle()
{
    int indices[3] = {0,1,2};
    int i = -1, j, n = 0;

    triangle.vertices = new Vertex[3];
    triangle.indices.indices = new unsigned int[3];
    triangle.vertexCount = 3;
    triangle.indices.count = 3;
    memcpy(triangle.indices.indices, indices, sizeof(indices));

    for (j = -1; j < 2; j += 2)
    {
        triangle.vertices[n].coords[0] = j;
        triangle.vertices[n].coords[1] = i;
        n++;
    }
    triangle.vertices[n].coords[0] = 0;
    triangle.vertices[n].coords[1] = 1;
}

void NestedShapes::initHexagon()
{
    int indices[12] = {0,1,2, 1,3,2, 2,3,4, 3,4,5};
    int i, n = 0;
    float j, f;

    hexagon.vertices = new Vertex[6];
    hexagon.indices.indices = new unsigned int[12];
    hexagon.vertexCount = 6;
    hexagon.indices.count = 12;
    memcpy(hexagon.indices.indices, indices, sizeof(indices));

    for (i = -1; i < 2; i++)
        for (j = -0.5; j < 1; j++)
        {
            i == 0 ? f = 2 * j : f = j;
            hexagon.vertices[n].coords[0] = f;
            hexagon.vertices[n].coords[1] = i;
            n++;
        }
}
