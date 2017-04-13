#include "glrenderer.h"
#include "../Logger/logger.h"

GlRenderer::GlRenderer(SdlGLContext& context):context(context)
{
    context.Execute(Task(&GlRenderer::init, this));
}

void GlRenderer::Start()
{
    if(terminated)
    {
        renderTask = Task(&GlRenderer::renderLoop, this);
        context.Execute(renderTask);
    }
}

void GlRenderer::Stop()
{
    terminated = true;
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

void GlRenderer::Draw(const Scene& path)
{
    std::lock_guard<std::mutex> lock(queueLock);
    if (renderQueue.size() < 3)
        renderQueue.push(path);
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
    terminated = false;
    Logger::Log("Renderer started");
    printContextInfo();
    viewportSize = context.getWindow().getSize();
    while(!terminated)
    {
        if(pause)
            pause.WaitFor(false);
        update();
        render();
        draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    Logger::Log("Renderer stopped");
}

void GlRenderer::update()
{
    ViewportSize size = viewportSize;
    gl::setViewport(size.width, size.height);
}

glhelpers::VertexArrayObject& GlRenderer::getVAO(GraphicsObject& obj)
{
    const std::string& name = obj.getAttribute("name");
    if (data.VAOs.count(name))
        glhelpers::updateDataBuffers(obj, data.VAOs[name]);
    else
        data.VAOs.emplace(name, glhelpers::initVAO(obj));
    return data.VAOs[name];
}

void GlRenderer::init()
{
    gl::setClearColor(0.2, 0.2, 0.2, 1);
    gl::enable(GL_BLEND);
    gl::enable(GL_DEPTH_TEST);
    //gl::enable(GL_CULL_FACE);
    gl::culling::setFrontFace(GL_CW);
    gl::culling::cullFace(GL_BACK);
    gl::depthbuffer::enableWrite(GL_TRUE);
    gl::depthbuffer::setFunction(GL_LEQUAL);
    gl::depthbuffer::setRange(0.0f, 1.0f);
    gl::setBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GlRenderer::render()
{
    if (!renderQueue.empty())
    {
        std::unique_lock<std::mutex> lock(queueLock);
        if (!renderQueue.empty())
        {
            currentScene = renderQueue.front();
            renderQueue.pop();
            emptyPath = false;
        }
    }
    if (!emptyPath)
    {
        std::vector<glhelpers::FramebufferObject> fbos;
        for (RenderStep& step : currentScene.path.steps)
        {
            fbos.push_back(glhelpers::initFramebuffer(step.target));
            if (step.target.target == RenderTargets::SCREEN)
                update();
            fbos.back().FBO.Bind();
            gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            step.preConfig(*step.prog);
            step.prog->setUniform(0, "useTex");

            for (int& id: step.objects)
            {
                GraphicsObject& obj = currentScene.objects[id];
                glhelpers::VertexArrayObject& VAO = getVAO(obj);
                for (auto& conf : obj.getConfigurationMap())
                    conf.second(*step.prog);
                step.postConfig(*step.prog);
                VAO.VAO.Bind();
                if (step.genericTexture.textureId != 0)
                    fbos[step.genericTexture.stepId].textures[step.genericTexture.textureId - 1].Bind();
                if (VAO.tex.getId() != 0)
                {
                    step.prog->setUniform(1, "useTex");
                    VAO.tex.Bind(1);
                }
                if (VAO.externTex)
                    fbos[VAO.stepId].textures[VAO.texId].Bind();
                step.prog->Use();
                gl::drawElements(GL_TRIANGLES, obj.data().indices.size(), GL_UNSIGNED_INT, nullptr);
                gl::program::use(0);
            }
        }
        for (glhelpers::FramebufferObject& fbo: fbos)
        {
            GLuint rb[1] = {fbo.depthBuff};
            gl::renderbuffer::erase(1, rb);
        }
    }
}

void GlRenderer::draw()
{
    SDL_GL_SwapWindow(context.getSdlWindow());
}
