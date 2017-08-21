#ifndef OPENGLSDL_H
#define OPENGLSDL_H

#include "glprogram.h"
#include "glshader.h"
#include "glrenderer.h"
#include "graphicscontextdata.h"
#include "../../../Core/service/service_cluster.h"

class GraphicsService: public Service
{
    Executor* serviceThread;
    SDL_GLContext parentContext;
    SdlWindow dummyWindow;
    void initContext();
    std::list<SdlGLContext> contexts;
    std::unique_ptr<GraphicsContextData> data{std::make_unique<GraphicsContextData>()};
public:
    ~GraphicsService();
    GraphicsService();
    void Start() override;
    void Stop() override;
    void Restart() override;
    void Pause() override;
    void Resume() override;
    void Wait() override;
    SdlGLContext& GetContext();
    Window GetWindow(const std::string& title, int x, int y);
};

class OpenGlSdl: public GraphicsApi
{
    SdlGLContext* dummyContext;
    GraphicsService* mainService;
    ServiceCluster& cluster;
    std::list<GlShader> shaders;
    std::list<GlProgram> programs;
public:
    OpenGlSdl(ServiceCluster& service);
    OpenGlSdl(OpenGlSdl&& src);
    OpenGlSdl(OpenGlSdl&) = delete;
    ~OpenGlSdl();
    Window CreateWindow(const std::string& title, int x, int y);
    Shader& CreateShader(const std::string& source, ShaderType type);
    Program& CreateProgram();
    Renderer& GetRenderer();
};

#endif // OPENGLSDL_H
