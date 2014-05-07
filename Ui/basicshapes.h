#ifndef BASICSHAPES_H
#define BASICSHAPES_H

#include "../Graphics/graphicsapi.h"


class NestedShapes
{
    RenderObject box;
    RenderObject triangle;
    RenderObject hexagon;
    void initBox();
    void initTriangle();
    void initHexagon();
    RenderObject getCopy(const RenderObject& src);
    NestedShapes();
public:
    friend class Shapes;
    RenderObject Triangle();
    RenderObject Box();
    RenderObject Hexagon();
};

class Shapes {
    static NestedShapes shapes;
public:
    Shapes();
    static RenderObject Box(){ return shapes.Box(); }
    static RenderObject Triangle(){ return shapes.Triangle(); }
    static RenderObject Hexagon(){ return shapes.Hexagon(); }
};
#endif // BASICSHAPES_H
