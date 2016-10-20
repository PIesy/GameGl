#ifndef BASICSHAPES_H
#define BASICSHAPES_H

#include "Graphics/graphicsapi.h"


class NestedShapes
{
    RenderData box;
    RenderData triangle;
    RenderData hexagon;
    void initBox();
    void initTriangle();
    void initHexagon();
    RenderData getCopy(const RenderData& src);
    NestedShapes();
public:
    friend class Shapes;
    RenderData Triangle();
    RenderData Box();
    RenderData Hexagon();
};

class Shapes {
    static NestedShapes shapes;
public:
    Shapes();
    static RenderData Box(){ return shapes.Box(); }
    static RenderData Triangle(){ return shapes.Triangle(); }
    static RenderData Hexagon(){ return shapes.Hexagon(); }
};
#endif // BASICSHAPES_H
