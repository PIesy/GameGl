#include "basicshapes.h"
#include <cstring>
#include <cstdlib>

NestedShapes Shapes::shapes;

NestedShapes::NestedShapes()
{
    initBox();
    initTriangle();
    initHexagon();
}

RenderData NestedShapes::Box()
{
    return getCopy(box);
}

RenderData NestedShapes::Triangle()
{
    return getCopy(triangle);
}

RenderData NestedShapes::Hexagon()
{
    return getCopy(hexagon);
}

RenderData NestedShapes::getCopy(const RenderData &src)
{
    RenderData obj;

    obj.indices = src.indices;
    obj.vertices = src.vertices;
    return obj;
}

void NestedShapes::initBox()
{
    box.indices = {3,2,0, 1,3,0,
                   7,3,1, 5,7,1,
                   2,6,7, 3,2,7,
                   5,7,6, 5,4,6,
                   1,0,4, 5,1,4,
                   4,6,2, 0,4,2};
    int i, j, c = 0;
    float z;
    Vertex tmp;

    Vec4 colors[8] = {{1,0,0,1}, {0,1,0,1}, {0,0,1,1}, {1,1,1,1},
                       {1,1,0,1}, {1,0,1,1}, {0.5,0,0,1}, {0,0.5,0,1}};

    for(z = 1; z > -2; z -= 2)
        for (i = -1; i < 2; i += 2)
            for (j = -1; j < 2; j += 2)
            {
                tmp.color = colors[c];
                c++;
                tmp.coords[0] = j;
                tmp.coords[1] = i;
                tmp.coords[2] = z;
                box.vertices.push_back(tmp);
            }
}

void NestedShapes::initTriangle()
{
    triangle.indices = {0,1,2};
    int i = -1, j;
    Vertex tmp;

    for (j = -1; j < 2; j += 2)
    {
        tmp.coords[0] = j;
        tmp.coords[1] = i;
        triangle.vertices.push_back(tmp);
    }
    tmp.coords[0] = 0;
    tmp.coords[1] = 1;
    triangle.vertices.push_back(tmp);
}

void NestedShapes::initHexagon()
{
    hexagon.indices = {0,1,2, 1,3,2, 2,3,4, 3,4,5};
    int i;
    float j, f;
    Vertex tmp;

    for (i = -1; i < 2; i++)
        for (j = -0.5; j < 1; j++)
        {
            i == 0 ? f = 2 * j : f = j;
            tmp.coords[0] = f;
            tmp.coords[1] = i;
            hexagon.vertices.push_back(tmp);
        }
}
