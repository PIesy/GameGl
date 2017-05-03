#include "glrenderer.h"
#include "../../../Logger/logger.h"
#include "gltexturefactory.h"

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
    gl::setClearColor(0.0, 0.0, 0.0, 0.0);
    gl::enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    gl::culling::setFrontFace(GL_CW);
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
        ViewportSize size = viewportSize;
        for (RenderStep& step : currentScene.path.steps)
        {
            glContext.UpdateState(step.attributes);
            gl::GlFrameBufferContainer& frameBuffer = glContext.BuildFrameBuffer(step, currentScene.path.steps, size.width, size.height);
            frameBuffer.frameBuffer.Bind();
            glContext.UpdateFrameBuffer(step.attributes);
            step.preConfig(*step.prog);

            for (int id : step.objects)
            {
                MeshDescriptor& mesh = currentScene.meshes[id];
                gl::GlMeshObject& meshObject = glContext.LoadMesh(mesh.mesh);
                mesh.configuration(*step.prog);
                step.postConfig(*step.prog);
                meshObject.vertexArray.Bind();

                int bindPoint = 0;
                for (Texture& genericTexture : step.genericTextures)
                    glContext.LoadTexture(genericTexture).Bind(bindPoint++);
                if (!meshObject.textures.empty())
                {
                    for (gl::GlTexture& texture : meshObject.textures)
                        texture.Bind(bindPoint++);
                }

                step.prog->Use();
                gl::drawElements(GL_TRIANGLES, mesh.mesh.GetInfo().indexCount, GL_UNSIGNED_INT, nullptr);
                gl::program::use(0);
            }
        }
        glContext.ClearFrameState();
    }
}

void GlRenderer::draw()
{
    SDL_GL_SwapWindow(context.getSdlWindow());
}
