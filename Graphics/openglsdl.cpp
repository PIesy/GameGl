#include "openglsdl.h"

GraphicsService::GraphicsService()
{
    serviceThread.setName("graphics service");
    dummyWindow.Open("dummy", 0, 0, true);
    Task initContext(&GraphicsService::initContext, this);
    serviceThread.setTask(initContext);
}

GraphicsService::~GraphicsService()
{
    serviceThread.Terminate();
    Wait();
}

void GraphicsService::initContext()
{
    parentContext = SDL_GL_CreateContext(dummyWindow);
    initGlew();
}

void GraphicsService::Start()
{
   Logger::Log("Graphics service started");
}

void GraphicsService::Stop()
{
    serviceThread.Terminate();
}

void GraphicsService::Restart() {}

void GraphicsService::Wait()
{
    serviceThread.Join();
}

void GraphicsService::Pause() {}

void GraphicsService::Resume() {}

SdlGLContext& GraphicsService::getContext()
{
    Task getContext;
    auto future = getContext.SetTask([this]
    {
        SDL_GLContext result = SDL_GL_CreateContext(dummyWindow);
        initGlew();
        SDL_GL_MakeCurrent(dummyWindow, parentContext);
        return result;
    });
    serviceThread.setTask(getContext);
    SDL_GLContext context = future.get();
    contexts.emplace_back(context, dummyWindow);
    return contexts.back();
}

OpenGlSdl::OpenGlSdl()
{
    if(!SDL_WasInit(SDL_INIT_VIDEO))
        SDL_Init(SDL_INIT_VIDEO);
    mainService = new GraphicsService();
    SdlGLContext& dummy = (SdlGLContext&)mainService->getContext();
    dummyContext = &dummy;
}

OpenGlSdl::~OpenGlSdl()
{
    delete mainService;
}

Window OpenGlSdl::CreateWindow(std::string title, int x, int y)
{
    SdlWindow* window = new SdlWindow();
    window->Open(title, x, y);
    Window wind(*window);
    return wind;
}

Shader& OpenGlSdl::CreateShader(std::string source, ShaderType type)
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

ServiceContainer OpenGlSdl::getService()
{
    cluster = new ServiceCluster();
    cluster->setMainService(mainService);
    return cluster;
}

Renderer& OpenGlSdl::GetRenderer()
{
    SdlGLContext& context = mainService->getContext();
    GlRenderer* renderer = new GlRenderer(context);
    cluster->Add(renderer);
    return *renderer;
}
