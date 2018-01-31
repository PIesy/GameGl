#include <algorithm>
#include "openglsdl.h"
#include "../../../Logger/logger.h"
#include "Wrappers/gldebug.h"
#include "gl_generic.h"
#include "gl_program_proxy.h"

const auto logger = Logger::GetLogger(getClassName<GraphicsService>());

GraphicsService::GraphicsService(ServiceCluster& cluster) : cluster(cluster) {}

GraphicsService::~GraphicsService()
{
    SDL_GL_DeleteContext(parentContext);
    Wait();
}

void GraphicsService::initContext()
{
    sharedGlData.Init(*data);
    parentContext = SDL_GL_CreateContext(*dummyWindow);
    gl::initGlew();
    started.SetValue(true);
}

void GraphicsService::Start()
{
    if (!SDL_WasInit(SDL_INIT_VIDEO))
        SDL_Init(SDL_INIT_VIDEO);
    dummyWindow = new SdlWindow("dummy", 0, 0, true);
    cluster.Add(dummyWindow);
    Task initContext(std::bind(&GraphicsService::initContext, this));
    serviceThread.Execute(initContext);
    logger.LogDebug("Graphics service started");
}

void GraphicsService::Stop() {}

void GraphicsService::Restart() {}

void GraphicsService::Wait() {}

void GraphicsService::Pause() {}

void GraphicsService::Resume() {}

void GraphicsService::WaitForStart()
{
    started.WaitFor(true);
}

SdlGLContext& GraphicsService::GetContext()
{
    Task getContext;
    auto future = getContext.SetTask([this]
    {
        SDL_GLContext result = SDL_GL_CreateContext(*dummyWindow);
        gl::initGlew();
        gl::registerDebugCallback();
        SDL_GL_MakeCurrent(*dummyWindow, parentContext);
        contexts.emplace_back(result, *dummyWindow, *data);
        return std::ref(contexts.back());
    });
    serviceThread.Execute(getContext);
    return future.get();
}

Window GraphicsService::GetWindow(const std::string& title, int x, int y)
{
    Task openWindow;
    auto future = openWindow.SetTask([&]
    {
        auto* window = new SdlWindow(title, x, y, false);
        cluster.Add(window);
        return window;
    });
    serviceThread.Execute(openWindow);
    return Window{*future.get()};
}

OpenGlSdl::OpenGlSdl(ServiceCluster& service) : cluster(service)
{
    if (!SDL_WasInit(SDL_INIT_VIDEO))
        SDL_Init(SDL_INIT_VIDEO);
    mainService = dynamic_cast<GraphicsService*>(&cluster.GetMainService());
    auto* tempService = mainService;
    initializedFuture = std::async(std::launch::async, [tempService]
    {
        tempService->WaitForStart();
        SdlGLContext* dummyContext = &tempService->GetContext();
        return dummyContext;
    });
}

Window OpenGlSdl::CreateWindow(const std::string& title, int x, int y)
{
    init();
    return mainService->GetWindow(title, x, y);
}

Shader& OpenGlSdl::CreateShader(const std::string& source, ShaderType type)
{
    init();
    Task createShader;
    auto future = createShader.SetTask([source, type, this]
    {
        GlProgramProxy proxy{{source, type}, *dummyContext};
        return proxy;
    });
    dummyContext->Execute(createShader);
    programs.emplace_back(future.get());
    return programs.back();
}

Renderer& OpenGlSdl::GetRenderer()
{
    init();
    SdlGLContext& context = mainService->GetContext();
    auto* renderer = new GlRenderer(context);
    cluster.Add(renderer);
    return *renderer;
}

OpenGlSdl::OpenGlSdl(OpenGlSdl&& src) noexcept : cluster(src.cluster)
{
    dummyContext = src.dummyContext;
    mainService = src.mainService;
    initializedFuture = std::move(src.initializedFuture);
}

void OpenGlSdl::init()
{
    std::call_once(initFlag, [this]
    {
        dummyContext = initializedFuture.get();
    });
}
