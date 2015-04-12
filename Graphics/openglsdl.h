#ifndef OPENGLSDL_H
#define OPENGLSDL_H

#include "renderdefs.h"
#include "graphicsapi.h"
#include "renderqueue.h"
#include "renderer.h"

class DummyGlContext
{
    DummyData data;
    Worker worker;
    bool ready = false;
public:
    DummyGlContext(SDL_GLContext context, SDL_Window* window);
    GlShader* getShader();
    GlProgram* getProgram();
    bool Ready();
};

class OpenGlWrapper
{
    DummyGlContext* context;
    GraphicsData* data;
    RenderQueue* queue;
    void initGlew();
    void startRenderer();
public:
    OpenGlWrapper(GraphicsData* data);
    ~OpenGlWrapper();
    Shader* CreateShader(std::string source, unsigned int type);
    GlProgram* CreateProgram();
    void SetData(Scene* scene);
    void setViewport(int x, int y, int width, int height);

    friend class OpenGlSdl;
    friend void setDummy(OpenGlWrapper* context);
};

class SdlWrapper
{
    GraphicsData* data;
    void initGlContext();
public:
    SdlWrapper(GraphicsData* data);
    void* CreateWindow(std::string title, int x, int y);
    friend class OpenGlSdl;
};

class OpenGlSdl: public GraphicsApi
{
    SdlWrapper* sdl;
    OpenGlWrapper* ogl;
    GraphicsData data;
public:
    OpenGlSdl();
    ~OpenGlSdl();
    void* CreateWindow(std::string title, int x, int y);
    Shader* CreateShader(std::string source, unsigned int type);
    Program* CreateProgram();
    void SendScene(void* scene);
    void setViewport(int x, int y, int width, int height);
};

#endif // OPENGLSDL_H
