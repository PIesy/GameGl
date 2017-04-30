#include "glrenderer.h"
#include "../../../Logger/logger.h"

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
            gl::GlFrameBufferContainer frameBuffer = glContext.BuildFrameBuffer(step.targets);
            frameBuffer.frameBuffer.Bind();
            if (frameBuffer.frameBuffer.GetId() == 0)
                update();
            gl::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            step.preConfig(*step.prog);
            step.prog->setUniform(0, "useTex");

            for (int id : step.objects)
            {
                MeshDescriptor& mesh = currentScene.meshes[id];
                gl::GlMeshObject& meshObject = glContext.LoadMesh(mesh.mesh);
                mesh.configuration(*step.prog);
                step.postConfig(*step.prog);
                meshObject.vertexArray.Bind();

                int bindPoint = 0;
                if (!step.genericTexture.info.name.empty())
                    globalData.Get().textureMap.at(step.genericTexture.info.name).Bind(bindPoint++);

                if (!meshObject.textures.empty())
                {
                    step.prog->setUniform(1, "useTex");
                    for (gl::GlTexture& texture : meshObject.textures)
                        texture.Bind(bindPoint++);
                }
                step.prog->Use();
                gl::drawElements(GL_TRIANGLES, mesh.mesh.GetInfo().indexCount, GL_UNSIGNED_INT, nullptr);
                gl::program::use(0);
            }
        }
    }
}

void GlRenderer::draw()
{
    SDL_GL_SwapWindow(context.getSdlWindow());
}
