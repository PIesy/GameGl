#include <algorithm>
#include "openglsdl.h"
#include "../../../Logger/logger.h"
#include "Wrappers/gldebug.h"

GraphicsService::GraphicsService()
{
    serviceThread = &core::core.Get().GetExecutor(true, getClassName<GraphicsService>());
    if (!SDL_WasInit(SDL_INIT_VIDEO))
        SDL_Init(SDL_INIT_VIDEO);
    dummyWindow.Open("dummy", 0, 0, true);
    Task initContext(std::bind(&GraphicsService::initContext, this));
    serviceThread->Execute(initContext);
}

GraphicsService::~GraphicsService()
{
    SDL_GL_DeleteContext(parentContext);
    Wait();
}

void GraphicsService::initContext()
{
    globalData.Init(*data);
    parentContext = SDL_GL_CreateContext(dummyWindow);
    initGlew();
}

void GraphicsService::Start()
{
   Logger::Log("Graphics service started");
}

void GraphicsService::Stop()
{
}

void GraphicsService::Restart() {}

void GraphicsService::Wait()
{
}

void GraphicsService::Pause() {}

void GraphicsService::Resume() {}

SdlGLContext& GraphicsService::GetContext()
{
    Task getContext;
    auto future = getContext.SetTask([this]
    {
        SDL_GLContext result = SDL_GL_CreateContext(dummyWindow);
        initGlew();
        gl::registerDebugCallback();
        SDL_GL_MakeCurrent(dummyWindow, parentContext);
        contexts.emplace_back(result, dummyWindow, *data);
        return std::ref(contexts.back());
    });
    serviceThread->Execute(getContext);
    return future.get();
}

Window GraphicsService::GetWindow(const std::string& title, int x, int y)
{
    Task openWindow;
    auto future = openWindow.SetTask([&]
    {
        SdlWindow* window = new SdlWindow();
        window->Open(title, x, y);
        return window;
    });
    serviceThread->Execute(openWindow);
    return *future.get();
}

OpenGlSdl::OpenGlSdl(ServiceCluster& service) : cluster(service)
{
    if (!SDL_WasInit(SDL_INIT_VIDEO))
        SDL_Init(SDL_INIT_VIDEO);
    mainService = dynamic_cast<GraphicsService*>(&cluster.GetMainService());
    SdlGLContext& dummy = mainService->GetContext();
    dummyContext = &dummy;
}

OpenGlSdl::~OpenGlSdl()
{
}

Window OpenGlSdl::CreateWindow(const std::string& title, int x, int y)
{
    return mainService->GetWindow(title, x, y);
}

Shader& OpenGlSdl::CreateShader(const std::string& source, ShaderType type)
{
    shaders.emplace_back(*dummyContext);
    GlShader& shader = shaders.back();
    shader.Create(source, type);
    return shader;
}

Program& OpenGlSdl::CreateProgram()
{
    programs.emplace_back(*dummyContext);
    return programs.back();
}

Renderer& OpenGlSdl::GetRenderer()
{
    SdlGLContext& context = mainService->GetContext();
    GlRenderer* renderer = new GlRenderer(context);
    cluster.Add(renderer);
    return *renderer;
}

OpenGlSdl::OpenGlSdl(OpenGlSdl&& src) : cluster(src.cluster)
{
    dummyContext = src.dummyContext;
    mainService = src.mainService;
    programs = std::move(src.programs);
    shaders = std::move(src.shaders);
}
