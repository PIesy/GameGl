#ifndef GRAPHICSAPI_H
#define GRAPHICSAPI_H

#include <string>
#include "Api/apibase.h"
#include "graphicsclasses.h"


class GraphicsApi: public ApiBase
{
public:
    virtual Window& CreateWindow(std::string title, int x, int y) = 0;
    virtual Shader& CreateShader(std::string source, ShaderType type) = 0;
    virtual Program& CreateProgram() = 0;
    virtual Renderer& GetRenderer() = 0;
    virtual ~GraphicsApi(){}
};

#endif // GRAPHICSAPI_H
