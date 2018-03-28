#include "sdlclasses.h"

void SdlWindow::Close()
{
    std::call_once(openedFlag, [this]
    {
        *terminate = true;
        SDL_DestroyWindow(window);
    });
}

WindowSize SdlWindow::GetSize()
{
    WindowSize result;
    SDL_GetWindowSize(window ,&result.width, &result.height);
    return result;
}

SdlWindow::operator SDL_Window*()
{
    return window;
}

void SdlWindow::Start()
{
    Task task{[&]()
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
        if (!hidden)
            window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED, width, height,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        else
            window = SDL_CreateWindow(name.c_str(), 0, 0, width, height,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
        opened.SetValue(true);
        //SDL_SetRelativeMouseMode(SDL_TRUE);
    }};
    worker.Execute(task);
    task.WaitTillFinished();

    auto term = terminate;
    worker.Execute(Task{[term]
    {
        while (!*term)
        {
            SDL_PumpEvents();
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1ms);
        }
    }});
}

void SdlWindow::WaitForStart()
{
    opened.WaitFor(true);
}

void SdlWindow::Stop()
{
    Close();
}

void SdlWindow::Pause()
{

}

void SdlWindow::Resume()
{

}

void SdlWindow::Restart()
{
    Stop();
    Start();
}

void SdlWindow::Wait()
{

}

SdlWindow::~SdlWindow()
{
    Close();
}

SdlWindow::SdlWindow(const std::string& name, int width, int height, bool hidden) : name(name), width(width), height(height), hidden(hidden) {}

SdlGLContext::SdlGLContext(const SDL_GLContext& context, SdlWindow& window, SharedGlContextData& data) : window(&window), context(context)
{
    SharedGlContextData* localData = &data;
    executor.Execute(Task([this, localData]
    {
        SDL_GL_MakeCurrent(*this->window, this->context);
        sharedGlData.Init(*localData);
    }));
}

SdlGLContext::~SdlGLContext()
{
    SDL_GL_DeleteContext(context);
}

void SdlGLContext::Destroy()
{
    if(isValid.getState())
    {
        isValid.GetOwnership();
        if (isValid.getState())
            isValid.setState(false);
        isValid.ReleaseOwnership();
    }
}

bool SdlGLContext::Execute(const Invokable& invokable)
{
    return executor.Execute(invokable);
}

void SdlGLContext::SetWindow(Window& window)
{
    this->window = &dynamic_cast<SdlWindow&>(window.getWindow());
}

void SdlGLContext::MakeCurrent()
{
    SDL_GL_MakeCurrent(*window, context);
}

Window SdlGLContext::GetWindow()
{
    return Window{*window};
}

SdlWindow& SdlGLContext::getSdlWindow()
{
    return *window;
}
