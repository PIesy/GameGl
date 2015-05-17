#include "openglsdl.h"

OpenGlSdl::OpenGlSdl()
{
    sdl = new SdlWrapper(&data);
    ogl = new OpenGlWrapper(&data);
}

OpenGlSdl::~OpenGlSdl()
{
    delete ogl;
    delete sdl;
}

void* OpenGlSdl::CreateWindow(std::string title, int x, int y)
{
    sdl->CreateWindow(title, x, y);
    sdl->initGlContext();
    ogl->initGlew();
    ogl->startRenderer();
    return data.window;
}

Shader& OpenGlSdl::CreateShader(std::string source, ShaderType type)
{
    return ogl->CreateShader(source, type);
}

Program& OpenGlSdl::CreateProgram()
{
    return ogl->CreateProgram();
}

void OpenGlSdl::SendScene(void* scene)
{
    ogl->SetData((Scene*)scene);
}

void OpenGlSdl::setViewport(int x, int y, int width, int height)
{
    ogl->setViewport(x, y, width, height);
}
