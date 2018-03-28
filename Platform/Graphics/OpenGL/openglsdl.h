#ifndef OPENGLSDL_H
#define OPENGLSDL_H

#include "glrenderer.h"
#include "graphics_context_data.h"
#include "../../../Core/service/service_cluster.h"
#include "../../../Public/apis.h"
#include "gl_program_proxy.h"

class GraphicsService: public Service
{
    Executor& serviceThread = core::core.Get().GetExecutor(true, getClassName<GraphicsService>());
    SDL_GLContext parentContext;
    SdlWindow* dummyWindow = nullptr;
    GenericCondition<bool> started{false};
    void initContext();
    std::list<SdlGLContext> contexts;
    std::unique_ptr<SharedGlContextData> data{std::make_unique<SharedGlContextData>()};
    ServiceCluster& cluster;
public:
    ~GraphicsService() override;
    explicit GraphicsService(ServiceCluster& cluster);
    void Start() override;
    void Stop() override;
    void Restart() override;
    void Pause() override;
    void Resume() override;
    void Wait() override;
    void WaitForStart() override;

    SdlGLContext& GetContext();
    Window GetWindow(const std::string& title, int x, int y);
};

class OpenGlSdl: public GraphicsApi
{
    SdlGLContext* dummyContext = nullptr;
    GraphicsService* mainService;
    ServiceCluster& cluster;
    std::list<GlProgramProxy> programs;
    std::future<SdlGLContext*> initializedFuture;

    std::once_flag initFlag;
    void init();
public:
    explicit OpenGlSdl(ServiceCluster& service);
    OpenGlSdl(OpenGlSdl&& src) noexcept;
    OpenGlSdl(const OpenGlSdl&) = delete;
    ~OpenGlSdl() override = default;
    Window CreateWindow(const std::string& title, int x, int y) override;
    Shader& CreateShader(const std::string& source, ShaderType type) override;
    Renderer& GetRenderer() override;
};

#endif // OPENGLSDL_H
