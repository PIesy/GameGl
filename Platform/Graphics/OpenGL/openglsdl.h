#ifndef OPENGLSDL_H
#define OPENGLSDL_H

#include "glprogram.h"
#include "glshader.h"
#include "glrenderer.h"
#include "graphicscontextdata.h"
#include "../../../Core/service/service_cluster.h"

class GraphicsService: public Service
{
    Worker serviceThread;
    SDL_GLContext parentContext;
    SdlWindow dummyWindow;
    void initContext();
    std::list<SdlGLContext> contexts;
    std::unique_ptr<GraphicsContextData> data{std::make_unique<GraphicsContextData>()};
public:
    ~GraphicsService();
    GraphicsService();
    void Start();
    void Stop();
    void Restart();
    void Pause();
    void Resume();
    void Wait();
    SdlGLContext& getContext();
};

class OpenGlSdl: public GraphicsApi
{
    SdlGLContext* dummyContext;
    GraphicsService* mainService;
    ServiceCluster* cluster;
    std::list<GlShader> shaders;
    std::list<GlProgram> programs;
public:
    OpenGlSdl();
    ~OpenGlSdl();
    Window CreateWindow(std::string title, int x, int y);
    Shader& CreateShader(std::string source, ShaderType type);
    Program& CreateProgram();
    Renderer& GetRenderer();
    ServiceContainer getService();
};

#endif // OPENGLSDL_H
