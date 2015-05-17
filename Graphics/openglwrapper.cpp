#include "openglsdl.h"
#include <cstdio>
#include <iostream>

void setGlew();

void setDummy(OpenGlWrapper* context);

OpenGlWrapper::OpenGlWrapper(GraphicsData *data)
{
    this->data = data;
}

OpenGlWrapper::~OpenGlWrapper()
{
    delete queue;
    delete context;
}

void OpenGlWrapper::initGlew()
{
    Task init(setGlew);
    data->worker.setTask(init);
    init.WaitTillFinished();
}

Shader& OpenGlWrapper::CreateShader(std::string source, ShaderType type)
{
    if(!context->Ready())
        while(!context->Ready())
            SDL_Delay(1);

    GlShader* sh = context->getShader();
    sh->Create(source, type);
    return *sh;
}

GlProgram& OpenGlWrapper::CreateProgram()
{
    if(!context->Ready())
        while(!context->Ready())
            SDL_Delay(1);

    return *context->getProgram();
}

void OpenGlWrapper::startRenderer()
{
    queue = new RenderQueue;
    RenderInitializer* i = new RenderInitializer{queue, data};

    data->worker.setTask(Task(setDummy, this));
    data->worker.setTask(Task(::startRenderer, i));
}

void OpenGlWrapper::SetData(Scene *scene)
{
    queue->Push(scene);
}

void OpenGlWrapper::setViewport(int, int, int width, int height)
{
    data->resizing.lock();
    data->dimensions[0] = width;
    data->dimensions[1] = height;
    data->resolutionChanged = true;
    data->resizing.unlock();
}

void setDummy(OpenGlWrapper* context)
{
   context->context = new DummyGlContext(context->data->dummyContext, context->data->window);
}

void setGlew()
{
    GLenum glew_err;

    glewExperimental = 1;
    glew_err = glewInit();
    if(glew_err != GLEW_OK)
        Logger::Log((const char*)glewGetErrorString(glew_err));
    glew_err = glGetError();
}
