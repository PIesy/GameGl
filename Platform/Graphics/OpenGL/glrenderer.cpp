#include "glrenderer.h"
#include "../../../Logger/logger.h"
#include "gltexturefactory.h"
#include "../../../Graphics/graphics_events.h"
#include "gl_generic.h"

const auto logger = Logger::GetLogger(getClassName<GlRenderer>());

GlRenderer::GlRenderer(SdlGLContext& context):context(context)
{
    context.Execute(Task(std::bind(&GlRenderer::init, this)));
}

void GlRenderer::Start()
{
    if (terminated)
    {
        renderTask = Task(std::bind(&GlRenderer::renderLoop, this));
        pause.SetValue(true);
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
    pause.SetValue(true);
}

void GlRenderer::Resume()
{
    pause.SetValue(false);
}

void GlRenderer::Draw(const Scene& path)
{
    std::lock_guard<std::mutex> lock(queueLock);
    if (renderQueue.size() < 3)
        renderQueue.push(path);
    if (firstUse)
    {
        firstUse = false;
        pause.SetValue(false);
    }
}

void GlRenderer::SetViewport(int width, int height)
{
    viewportSize = {width, height};
}

void GlRenderer::SetWindow(Window& window)
{
    Stop();
    context.Execute(Task([this, &window] { context.SetWindow(window); context.MakeCurrent(); }));
    Start();
}

void GlRenderer::renderLoop()
{
    terminated = false;
    logger.LogDebug("Renderer started");
    printContextInfo();
    viewportSize = context.GetWindow().GetSize();
    while (!terminated)
    {
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        if (pause)
            pause.WaitFor(false);
        update();
        render();
        draw();
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        core::core->GetEventHandler().ThrowEvent(FrameInfoEvent{{duration}, 0});
    }
    logger.LogDebug("Renderer stopped");
}

void GlRenderer::update()
{
    ViewportSize size = viewportSize;
    gl::setViewport(size.width, size.height);
}

void GlRenderer::init()
{
    gl::setClearColor(1.0, 0.0, 0.0, 0.0);
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
            step.preConfig();

            for (int id : step.objects)
            {
                MeshDescriptor& mesh = currentScene.meshes[id];
                gl::GlMeshObject& meshObject = glContext.LoadMesh(mesh.mesh);
                mesh.configuration();
                step.postConfig();
                meshObject.vertexArray.Bind();
                gl::GlProgramPipeline& pipeline = glContext.GetProgramPipeline(step.shaders);
                pipeline.Validate();

                int bindPoint = 0;
                std::vector<LockedResource<gl::GlTexture>> textureHolder;
                for (Texture& genericTexture : step.genericTextures)
                {
                    textureHolder.push_back(glContext.AcquireTexture(genericTexture, true));
                    textureHolder.back().GetObject().Bind(bindPoint++);
                }
                if (!mesh.mesh.GetTextures().empty())
                {
                    for (const Texture& texture : mesh.mesh.GetTextures())
                    {
                        textureHolder.push_back(glContext.AcquireTexture(texture, true));
                        textureHolder.back().GetObject().Bind(bindPoint++);
                    }
                }

                pipeline.Bind();
                //logger.LogDebug(std::string("Pipeline Info:") + pipeline.GetInfo());
                gl::drawElements(GL_TRIANGLES, mesh.mesh.GetInfo().indexCount, GL_UNSIGNED_INT, nullptr);
                pipeline.Unbind();
            }
            glContext.UpdateFrameState(frameBuffer, step.attributes);
        }
        glContext.ClearFrameState();
        emptyPath = true;
    }
}

void GlRenderer::draw()
{
    SDL_GL_SwapWindow(context.getSdlWindow());
}
