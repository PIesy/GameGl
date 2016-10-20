#include "renderer.h"
#include "Logger/logger.h"

GlRenderer::GlRenderer(SdlGLContext& context):context(context)
{
    context.Execute(Task(&GlRenderer::init, this));
}

void GlRenderer::Start()
{
    if(terminate)
    {
        renderTask = Task(&GlRenderer::renderLoop, this);
        context.Execute(renderTask);
    }
}

void GlRenderer::Stop()
{
    terminate = true;
}

void GlRenderer::Restart()
{
    Stop();
    Start();
}

void GlRenderer::Wait()
{
    renderTask.WaitTillFinished();
}

void GlRenderer::Pause()
{
    pause.setValue(true);
}

void GlRenderer::Resume()
{
    pause.setValue(false);
}

void GlRenderer::Draw(const Scene& scene)
{
    std::lock_guard<std::mutex> lock(queueLock);
    renderQueue.emplace(scene);
}

void GlRenderer::SetViewport(int width, int height)
{
    viewportSize = {width, height};
}

void GlRenderer::SetWindow(const Window& window)
{
    Stop();
    context.Execute(Task([this, &window] { context.SetWindow(window); context.MakeCurrent(); }));
    Start();
}

void GlRenderer::renderLoop()
{
    terminate = false;
    Logger::Log("Renderer started");
    printContextInfo();
    viewportSize = context.getWindow().getSize();
    while(!terminate)
    {
        if(pause)
            pause.WaitFor(false);
        update();
        render();
        draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    Logger::Log("Renderer stoped");
}

void GlRenderer::update()
{
    ViewportSize size = viewportSize;
    glViewport(0, 0, size.width, size.height);
    //printGlError("Viewport update error " + std::to_string(size.width) + " " + std::to_string(size.height) + " ");
}

void GlRenderer::init()
{
    glClearColor(0,0,0,1);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    printGlError("Buffer error");
}

void GlRenderer::render()
{
    int i = 0;

    if (!renderQueue.empty())
    {
        std::unique_lock<std::mutex> lock(queueLock);
        if (!renderQueue.empty())
        {
            currentScene = renderQueue.front();
            renderQueue.pop();
            empty = false;
        }
    }
    if(!empty)
        for (i = 0; i < currentScene.passes; i ++)
        {
            const std::string& name = currentScene.objects[i].getAttribute("name");
            if (data.VAOs.count(name))
            {
                glhelpers::updateDataBuffers(currentScene.objects[i], data.VAOs[name]);
            }
            else
            {
                data.VAOs.insert({name, glhelpers::initVAO(currentScene.objects[i])});
            }
            glhelpers::VertexArrayObject& VAO = data.VAOs[name];
            glBindVertexArray(VAO.VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAO.IBO);
            printGlError("Bind error");
            currentScene.objects[i].data().program->Use();
            printGlError("Shader error");
            glDrawElements(GL_TRIANGLES, currentScene.objects[i].data().indices.size(), GL_UNSIGNED_INT, nullptr);
            printGlError("Draw error");
        }
}

void GlRenderer::draw()
{
    SDL_GL_SwapWindow(context.getSdlWindow());
    glClear((GLbitfield)(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
