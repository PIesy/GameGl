#include "sdlclasses.h"
#include "../../../Helpers/task.h"
#include <chrono>


void SdlWindow::Open(std::string name, int width, int height, bool hidden)
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

        //SDL_SetRelativeMouseMode(SDL_TRUE);
    }};
    worker->Execute(task);
    task.WaitTillFinished();

    using namespace std::chrono_literals;
    worker->Execute(Task{[]()
        {
            while (1)
            {
                SDL_PumpEvents();
                std::this_thread::sleep_for(1ms);
            }
        }
    });
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

SdlWindow::operator SDL_Window*()
{
    return window;
}

void SdlWindow::Start()
{

}

void SdlWindow::Stop()
{

}

void SdlWindow::Pause()
{

}

void SdlWindow::Resume()
{

}

void SdlWindow::Restart()
{

}

void SdlWindow::Wait()
{

}

SdlWindow::SdlWindow()
{
    worker = &core::core.Get().GetExecutor(true, "WindowExec");
}

SdlGLContext::SdlGLContext(const SDL_GLContext& context, const SdlWindow& window, GraphicsContextData& data) : window(window), context(context)
{
    GraphicsContextData* localData = &data;
    executor.Execute(Task([this, localData]
                        {
                            SDL_GL_MakeCurrent(this->window, this->context);
                            globalData.Init(*localData);
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
        if(isValid.getState())
            isValid.setState(false);
        isValid.ReleaseOwnership();
    }
}

void SdlGLContext::Execute(const Invokable& invokable)
{
    executor.Execute(invokable);
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
