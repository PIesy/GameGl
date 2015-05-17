#ifndef SDLCLASSES
#define SDLCLASSES

#include "graphicsclasses.h"

class SdlWindow: public Window
{
public:
    void Close();
};

class SdlGLContext: public RenderingContext
{

};

#endif // SDLCLASSES

