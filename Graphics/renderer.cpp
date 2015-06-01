#include "renderer.h"

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
    printGlError("Viewport update error " + std::to_string(size.width) + " " + std::to_string(size.height) + " ");
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
    glGenBuffers(1, &data.VBO);
    glGenBuffers(1, &data.IBO);
    glGenVertexArrays(1, &data.VAO);
    glBindVertexArray(data.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.IBO);
    glBufferData(GL_ARRAY_BUFFER, 50000 * sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 50000 * sizeof(unsigned), NULL, GL_DYNAMIC_DRAW);
    printGlError("Buffer error");
}

void GlRenderer::render()
{
    int i = 0;

    if (!renderQueue.empty())
    {
        currentScene = renderQueue.front();
        renderQueue.pop();
        empty = false;
    }
    if(!empty)
        for (i = 0; i < currentScene.passes; i ++)
        {
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            currentScene.objects[i]->data().vertices.size() * sizeof(Vertex),
                            currentScene.objects[i]->data().vertices.data());
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, coords));
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                            currentScene.objects[i]->data().indices.size() * sizeof(unsigned),
                            currentScene.objects[i]->data().indices.data());
            currentScene.objects[i]->data().program->Use();
            printGlError("SubData error");
            glDrawElements(GL_TRIANGLES, currentScene.objects[i]->data().indices.size(), GL_UNSIGNED_INT, nullptr);
            printGlError("Draw error");
        }
}

void GlRenderer::draw()
{
    SDL_GL_SwapWindow(context.getSdlWindow());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
