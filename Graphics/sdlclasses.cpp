#include "sdlclasses.h"


void SdlWindow::Open(std::string name, int width, int height, bool hidden)
{

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    if(!hidden)
        window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    else
        window = SDL_CreateWindow(name.c_str(), 0, 0, width, height,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
}

void SdlWindow::Close()
{
    SDL_DestroyWindow(window);
}

WindowSize SdlWindow::getSize()
{
    WindowSize result;
    SDL_GetWindowSize(window ,&result.width, &result.height);
    return result;
}

SdlWindow::operator SDL_Window *()
{
    return window;
}


SdlGLContext::SdlGLContext(const SDL_GLContext& context, const SdlWindow& window)
{
    worker.setName("opengl context");
    this->window = window;
    this->context = context;
    worker.setTask(Task([this] { SDL_GL_MakeCurrent(this->window, this->context); }));
}

SdlGLContext::~SdlGLContext()
{
    worker.Terminate();
    worker.Join();
    SDL_GL_DeleteContext(context);
}

void SdlGLContext::Destroy()
{
    if(isValid.getState())
    {
        isValid.GetOwnership();
        if(isValid.getState())
        {
            isValid.setState(false);
            worker.Terminate();
        }
        isValid.ReleaseOwnership();
    }
    worker.Join();
}

void SdlGLContext::Execute(const Invokable& invokable)
{
    worker.setTask(invokable);
}

void SdlGLContext::SetWindow(const Window& window)
{
    this->window = dynamic_cast<const SdlWindow&>(window.getWindow());
}

void SdlGLContext::MakeCurrent()
{
    SDL_GL_MakeCurrent(window, context);
}

Window SdlGLContext::getWindow()
{
    return window;
}

SdlWindow& SdlGLContext::getSdlWindow()
{
    return window;
}
