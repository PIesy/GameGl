#include "openglsdl.h"
#include <cstdio>

void initContext(void* data);
void setGlew();

DummyGlContext::DummyGlContext(SDL_GLContext context, SDL_Window* window)
{
    std::unique_lock<std::mutex> lock(data.lock);

    data.context = context;
    data.window = window;
    worker.setWork(initContext, &data);
    data.finish.wait(lock);
    ready = true;
}

GlProgram* DummyGlContext::getProgram()
{
    GlProgram* prog = new GlProgram(&worker);
    return prog;
}

GlShader* DummyGlContext::getShader()
{
    GlShader* sh = new GlShader(&worker);
    return sh;
}

bool DummyGlContext::Ready()
{
    return ready;
}

void initContext(void *data)
{
    DummyData* gl = (DummyData*)data;
    SDL_GL_MakeCurrent(gl->window, gl->context);
    gl->finish.notify_all();
}
