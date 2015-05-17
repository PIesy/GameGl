#include "openglsdl.h"

void createGlContext(GraphicsData* data);

SdlWrapper::SdlWrapper(GraphicsData* data)
{
    this->data = data;
    if(!SDL_WasInit(SDL_INIT_VIDEO))
        SDL_Init(SDL_INIT_VIDEO);
}

void* SdlWrapper::CreateWindow(std::string title, int x, int y)
{
    data->window = SDL_CreateWindow(title.c_str(),
                                         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                         x, y, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN |
                                         SDL_WINDOW_RESIZABLE);
    return data->window;
}

void SdlWrapper::initGlContext()
{
    Task init(createGlContext, data);
    data->worker.setTask(init);
    init.WaitTillFinished();
}

void createGlContext(GraphicsData *data)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    data->dummyContext = SDL_GL_CreateContext(data->window);
    data->context = SDL_GL_CreateContext(data->window);
}
