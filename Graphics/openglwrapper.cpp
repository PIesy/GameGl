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
    data->worker.setTask(Task(setGlew));
}

Shader* OpenGlWrapper::CreateShader(std::string source, unsigned int type)
{
    if(!context->Ready())
        while(!context->Ready())
            SDL_Delay(1);

    GlShader* sh = context->getShader();
    sh->Create(source, type);
    return sh;
}

GlProgram* OpenGlWrapper::CreateProgram()
{
    if(!context->Ready())
        while(!context->Ready())
            SDL_Delay(1);

    return context->getProgram();
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

void OpenGlWrapper::setViewport(int x, int y, int width, int height)
{
    data->resizing.lock();
    data->resolutionChanged = true;
    data->dimensions[0] = width;
    data->dimensions[1] = height;
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
        fprintf(stderr, "%s\n", glewGetErrorString(glew_err));
    glew_err = glGetError();
}
