#include <iostream>
#include "Public/coreinterface.h"
#include "Public/ui.h"
#include "Tests/tests.h"
#include "Framework/world.h"
#include "Framework/scenebuilder.h"
#include "Framework/Loaders/meshloader.h"
#include "Framework/Loaders/programloader.h"
#include "Logger/logger.h"
#include "Framework/texturebuilder.h"
#include <algorithm>

float aspectFactor;
bool stop = false;
Camera cam;
Vec3 lookDir = {0, -1, 1};
Vec3 camPos = {500, 500, 500};
Program* prog2;
Program* prog;
Program* skyboxProg;
Program* hdrProg;
Program* pbrProg;
Program* normalsProg;
Program* hdrIblProg;
Program* hdrIblConvProg;
Program* hdrIblPrefProg;
Program* brdfLutProg;
Texture cubeMap;
Texture pbrArrayCerberus;
Texture pbrArrayGold;
Light light1;
Texture hdrMap;
Texture hdrSkybox;
Texture hdrSkyboxConv;
Texture hdrSkyboxPref;
Texture brdfLut;
float windowSizeX = 1280.0f, windowSizeY = 720.0f;

void updateViewport(WindowEvent* e, Renderer* renderer);
void closeApp(void*, CoreInterface* core);
void shiftKeyboard(KeyboardEvent* event, World& world);
void mouseRotate(MouseEvent* event, DrawableWorldObject& obj);

Program& prepareProgram(CoreInterface* engine);
void initRenderer(World& world, Renderer& renderer);
void drawMesh2(World& world, Renderer& renderer);

Program& prepareProgram2(CoreInterface* engine);

int main()
{
    CoreInterface engine;
    Renderer& renderer = engine.Video()->GetRenderer();
    Action<WindowEvent*> setViewport(updateViewport, std::placeholders::_1, &renderer);
    Action<WindowEvent*> endGame(closeApp, std::placeholders::_1, &engine);
    aspectFactor = windowSizeX / windowSizeY;
    engine.Start();
    eventsHandlerTest();
    engine.getEventHandler().setListener<WindowEvent>(endGame, [](EventInterface* e) { return e->GetHint() == integral(WindowData::Type::Close); });
    engine.getEventHandler().setListener<WindowEvent>(setViewport, [](EventInterface* e) { return e->GetHint() == integral(WindowData::Type::Resize); });
    engine.getEventHandler().setListener<KeyboardEvent>(endGame, [](EventInterface* e) { return e->GetHint() == SDL_SCANCODE_Q; });
    Window window = engine.Video()->CreateWindow("Hello", windowSizeX, windowSizeY);
    renderer.SetWindow(window);
    UiLayer ui(engine.getCore(), window);
    ProgramLoader programLoader{engine.Resources(), *engine.Video()};
    prog = &prepareProgram(&engine);
    prog2 = &prepareProgram2(&engine);
    skyboxProg = &programLoader.Load("skybox_vertex_shader.glsl", "skybox_fragment_shader.glsl");
    hdrProg = &programLoader.Load("hdr_vertex_shader.glsl", "hdr_fragment_shader.glsl");
    pbrProg = &programLoader.Load("pbr_vertex_shader.glsl", "pbr_fragment_shader.glsl");
    normalsProg = &programLoader.Load("normals_vertex_shader.glsl", "normals_fragment_shader.glsl", "normals_geometry_shader.glsl");
    hdrIblProg = &programLoader.Load("ibl_cubemap_vertex_shader.glsl", "ibl_cubemap_fragment_shader.glsl", "ibl_cubemap_geometry_shader.glsl");
    hdrIblConvProg = &programLoader.Load("ibl_cubemap_vertex_shader.glsl", "ibl_cubemap_convolution_fragment_shader.glsl", "ibl_cubemap_geometry_shader.glsl");
    hdrIblPrefProg = &programLoader.Load("ibl_cubemap_vertex_shader.glsl", "ibl_cubemap_prefilter_fragment_shader.glsl", "ibl_cubemap_geometry_shader.glsl");
    brdfLutProg = &programLoader.Load("hdr_vertex_shader.glsl", "brdf_lut_fragment_shader.glsl");

    MeshLoader loader{engine.GetStorage()};
    TextureLoader textureLoader{engine.GetStorage()};

    cubeMap = textureLoader.LoadCubemap({"posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg"});
    pbrArrayCerberus = textureLoader.LoadArray({"cerberus/Textures/Cerberus_N.tga", "cerberus/Textures/Cerberus_A.tga",
                                                "cerberus/Textures/Cerberus_M.tga", "cerberus/Textures/Cerberus_R.tga"});
    pbrArrayGold = textureLoader.LoadArray({"pbr/gold/normal.png", "pbr/gold/albedo.png",
                                            "pbr/gold/metallic.png", "pbr/gold/roughness.png"});
    hdrMap = textureLoader.Load("lakeR.hdr", true);
    auto meshes = loader.Load("cerberus/Cerberus_LP.FBX");
    Mesh plane = Shapes::Plane(1000, 1000, engine.GetStorage(), "floor");
    Mesh skybox = loader.Load("cube.obj")[0];
    Mesh sphere = loader.Load("sphere.blend")[0];
    std::vector<Mesh> sofa = loader.Load("sofa/sofa.obj");
    meshes[0].AddTexture(pbrArrayCerberus);
    sphere.AddTexture(pbrArrayGold);
    std::for_each(std::begin(sofa), std::end(sofa), [](Mesh& m) {m.AddTexture(pbrArrayGold);});

    World world;
    plane.AddTexture(pbrArrayGold);

    cam = world.AddCamera(camPos);
    cam.SetAspectRatio(aspectFactor);
    cam.SetNearPlane(1.0f);
    cam.SetFarPlane(1700.f);
    cam.SetLookDirection(lookDir);

    light1 = world.AddLight({450, 410, 450}, {150, 150, 150});
    world.AddLight({350, 410, 350}, {150, 150, 150});
    world.AddLight({450, 440, 450}, {150, 150, 150});
    world.AddLight({350, 440, 350}, {150, 150, 150});

    DrawableWorldObject& obj = world.AddObject(meshes[0], {400, 420, 400});
    obj.SetScale(1);
    obj.Rotate(270.0f, {1, 0, 0});
    //world.AddObject(plane, {0, 400, 0});
    world.AddObject(sofa, {450, 420, 450}).SetScale(0.05f);
    world.AddObject(sphere, {350, 400, 350}).SetScale(10);
    world.AddObject(sphere, {350, 420, 350}).SetScale(10);
    world.AddObject(sphere, {350, 440, 350}).SetScale(10);
    DrawableWorldObject& s = world.AddObject(skybox, {0, 0, 0});
    s.SetRenderFlags(RenderFlags::SkyBox);

    world.AddObject(Shapes::Rectangle(engine.GetStorage()), {0, 0, 0}).SetRenderFlags(RenderFlags::FullScreenQuad);

    Action<MouseEvent*> rotate(mouseRotate, std::placeholders::_1, std::ref(obj));
    Action<KeyboardEvent*> move(shiftKeyboard, std::placeholders::_1, std::ref(world));

    engine.getEventHandler().setListener<KeyboardEvent>(move);
    engine.getEventHandler().setListener<MouseEvent>(rotate, [](EventInterface* e) { return e->GetHint() == integral(MouseData::Type::Motion); });

    initRenderer(world, renderer);
    while (!stop)
    {
        obj.Rotate(0.5f, {0, 1, 0});
        drawMesh2(world, renderer);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return engine.WaitEnd();
}

void closeApp(void*, CoreInterface* core)
{
    Logger::Log("The end is coming");
    stop = true;
    core->Stop();
}

void updateViewport(WindowEvent* e, Renderer* renderer)
{
    Logger::Log("Viewport updated");
    aspectFactor = (float)e->getPayload().coordinates[0] / e->getPayload().coordinates[1];
    Logger::Log(std::to_string(aspectFactor));
    windowSizeX = e->getPayload().coordinates[0];
    windowSizeY = e->getPayload().coordinates[1];
    renderer->SetViewport(e->getPayload().coordinates[0], e->getPayload().coordinates[1]);
}

void shiftKeyboard(KeyboardEvent* event, World& world)
{
    float deg = 5.0f;
    static Vec3 wOffset = Vec3(0.0f);

    Mat3 yRotate = rotationMatrix({0, 1, 0}, deg);
    Mat3 nyRotate = rotationMatrix({0, 1, 0}, -deg);
    Mat3 vRotate = rotationMatrix(glm::cross(Vec3{0, 1, 0}, lookDir), deg);
    Mat3 nvRotate = rotationMatrix(glm::cross(Vec3{0, 1, 0}, lookDir), -deg);

    if (event->getPayload().scancode == SDL_SCANCODE_D)
        wOffset.x += 1.0f;
    if (event->getPayload().scancode == SDL_SCANCODE_A)
        wOffset.x -= 1.0f;
    if (event->getPayload().scancode == SDL_SCANCODE_R)
        wOffset.z += 1.0f;
    if (event->getPayload().scancode == SDL_SCANCODE_F)
        wOffset.z -= 1.0f;

    if (event->getPayload().scancode == SDL_SCANCODE_RIGHT)
        lookDir = nyRotate * lookDir;
    if (event->getPayload().scancode == SDL_SCANCODE_LEFT)
        lookDir = yRotate * lookDir;
    if (event->getPayload().scancode == SDL_SCANCODE_UP)
        lookDir = nvRotate * lookDir;
    if (event->getPayload().scancode == SDL_SCANCODE_DOWN)
        lookDir = vRotate * lookDir;

    if (event->getPayload().scancode == SDL_SCANCODE_SPACE)
        camPos.y += 1;

    if (event->getPayload().scancode == SDL_SCANCODE_C)
        camPos.y -= 1;

    if (event->getPayload().scancode == SDL_SCANCODE_RETURN)
    {
        lookDir = {1, 0, 1};
        camPos = {50, 50, 50};
    }
    lookDir = glm::normalize(lookDir);
    Vec2 offset = glm::normalize(Vec2{lookDir.x, lookDir.z});
    if (event->getPayload().scancode == SDL_SCANCODE_W)
    {
        camPos.x += 1 * offset.x;
        camPos.z += 1 * offset.y;
    }
    if (event->getPayload().scancode == SDL_SCANCODE_S)
    {
        camPos.x -= 1 * offset.x;
        camPos.z -= 1 * offset.y;
    }

    cam.SetLookDirection(lookDir);
    cam.SetPosition(camPos);
}

void mouseRotate(MouseEvent *event, DrawableWorldObject& obj)
{
    float degree = std::sqrt(std::pow((float)event->getPayload().relativeCoordinates[0], 2.0f) +
                                     std::pow((float)event->getPayload().relativeCoordinates[1], 2.0f));
    Vec3 axis = {event->getPayload().relativeCoordinates[1] / degree, event->getPayload().relativeCoordinates[0] / degree, 0};
    if(event->getPayload().state)
        obj.Rotate(degree, axis);
}

Program& prepareProgram(CoreInterface* engine)
{
    ProgramLoader loader{engine->Resources(), *engine->Video()};
    return loader.Load("VertexShader.glsl", "FragmentShader.glsl");
}

Program& prepareProgram2(CoreInterface* engine)
{
    ProgramLoader loader{engine->Resources(), *engine->Video()};
    return loader.Load("cubemap_vertex_shader.glsl", "cubemap_fragment_shader.glsl", "cubemap_geometry_shader.glsl");
}

void initRenderer(World& world, Renderer& renderer)
{
    SceneBuilder builder;
    RenderStep step0;
    RenderStep step1;
    RenderStep step3;
    Light fakeLight;

    hdrSkybox = TextureBuilder::BuildCubemap(TextureBindpoint::Color, 1024, 1024, 1, 4, TexturePixelFormat::Float16, true);
    hdrSkybox.info.name = "hdrSkybox";

    step0.genericTextures = {hdrMap};
    step0.prog = hdrIblProg;
    step0.targets.push_back(RenderTarget(hdrSkybox));
    step0.attributes.push_back(RenderingAttribute::UpdateMipmaps);
    step0.postConfig = [=](Program& p)
    {
        p.SetUniform(fakeLight.GetCubeMapMatrices()[0], "projections", 6);
        p.SetUniform(fakeLight.GetPerspectiveMatrix(), "perspective");
    };

    step1.genericTextures = {hdrSkybox};
    step1.prog = hdrIblConvProg;
    hdrSkyboxConv = TextureBuilder::BuildCubemap(TextureBindpoint::Color, 32, 32, 1, 4, TexturePixelFormat::Float16, false);
    hdrSkyboxConv.info.name = "hdrSkyboxDiffuse";
    step1.targets.push_back(RenderTarget(hdrSkyboxConv));
    step1.postConfig = [=](Program& p)
    {
        p.SetUniform(fakeLight.GetCubeMapMatrices()[0], "projections", 6);
        p.SetUniform(fakeLight.GetPerspectiveMatrix(), "perspective");
    };

    builder.AddStep(step0, RenderFlags::SkyBox);
    builder.AddStep(step1, RenderFlags::SkyBox);

    const int mipmaps = 5;

    hdrSkyboxPref = TextureBuilder::BuildCubemap(TextureBindpoint::Color, 128, 128, 1, 4, TexturePixelFormat::Float16, false);
    hdrSkyboxPref.info.mipmaps = mipmaps - 1;
    hdrSkyboxPref.info.name = "hdrSkyboxPref";
    for (int i = 0; i < mipmaps; ++i)
    {
        RenderStep step;

        hdrSkyboxPref.parameters.useMipLevel = i;
        step.genericTextures = {hdrSkybox};
        step.targets.push_back(RenderTarget(hdrSkyboxPref));
        step.prog = hdrIblPrefProg;
        step.postConfig = [=](Program& p)
        {
            p.SetUniform(fakeLight.GetCubeMapMatrices()[0], "projections", 6);
            p.SetUniform(fakeLight.GetPerspectiveMatrix(), "perspective");
            p.SetUniform((float) i / mipmaps, "roughness");
        };
        builder.AddStep(step, RenderFlags::SkyBox);
    }

    brdfLut = TextureBuilder::Build2DTexture(TextureBindpoint::Color, 512, 512, 1, 3, TexturePixelFormat::Float16, false);
    brdfLut.info.name = "brdfLut";
    step3.prog = brdfLutProg;
    step3.attributes = {RenderingAttribute::ClearBuffer};
    step3.targets.push_back(RenderTarget(brdfLut));

    builder.AddStep(step3, RenderFlags::FullScreenQuad);

    renderer.Draw(builder.BuildScene(world, cam));
}

void drawMesh2(World& world, Renderer& renderer)
{
    SceneBuilder builder;
    RenderStep step1;
    RenderStep step2;
    RenderStep step2_5;
    RenderStep step3;
    RenderStep step4;
    Texture texture = TextureBuilder::BuildCubemap(TextureBindpoint::Depth, 512, 512, world.GetLights().size(), 1, TexturePixelFormat::Float24, false);
    texture.info.name = "shadow";

    step1.targets.push_back(RenderTarget(texture));
    step1.prog = prog2;
    step1.attributes = {RenderingAttribute::DepthTest, RenderingAttribute::ClearBuffer};

    int i = 0;
    for (Light& light : world.GetLights())
    {
        if (i != 0)
        {
            step1.attributes = {RenderingAttribute::DepthTest};
            step1.frameBufferProperties.frameBufferType = FrameBufferProperties::REUSE_FROM_STEP;
            step1.frameBufferProperties.stepId = 0;
        }
        step1.postConfig = [light, i](Program& p)
        {
            p.SetUniform(i, "baseLayer");
            p.SetUniform(light.GetCubeMapMatrices()[0], "projections", 6);
            p.SetUniform(light.GetPerspectiveMatrix(), "perspective");
            p.SetUniform(light.GetMaxDistance(), "farPlane");
            p.SetUniform(light.GetPosition(), "lightPosition");
        };
        builder.AddStep(step1);
        i++;
    }

    Texture hdrTexture;
    hdrTexture.info.name = "colorBuffHdr";
    hdrTexture.info.targetPixelFormat= TexturePixelFormat::Float32;
    hdrTexture.info.channels = 4;
    hdrTexture.info.width = windowSizeX;
    hdrTexture.info.height = windowSizeY;

    step2.genericTextures = {hdrSkyboxConv, hdrSkyboxPref, brdfLut, texture};
    step2.targets.push_back(RenderTarget(hdrTexture));
    step2.prog = pbrProg;

    step2_5.attributes = {RenderingAttribute::DepthTest};
    step2_5.frameBufferProperties.frameBufferType = FrameBufferProperties::REUSE_FROM_STEP;
    step2_5.frameBufferProperties.stepId = i;
    step2_5.prog = normalsProg;

    step3.frameBufferProperties.frameBufferType = FrameBufferProperties::REUSE_FROM_STEP;
    step3.frameBufferProperties.stepId = i;
    cubeMap.parameters.sampling = TextureSampling::Linear;
    cubeMap.parameters.anisotropicFiltering = 16;
    step3.genericTextures = {hdrSkybox};
    step3.attributes = {RenderingAttribute::DepthTest};
    step3.prog = skyboxProg;

    step3.postConfig = [=](Program& p)
    {
        p.SetUniform(cam.GetPerspectiveMatrix(), "perspective");
        p.SetUniform(cam.GetCameraMatrix(), "WtoCMatrix");
    };

    step4.genericTextures = {hdrTexture};
    step4.prog = hdrProg;

    //builder.AddStep(step1);
    builder.AddStep(step2);
    //builder.AddStep(step2_5);
    builder.AddStep(step3, RenderFlags::SkyBox);
    builder.AddStep(step4, RenderFlags::FullScreenQuad);

    renderer.Draw(builder.BuildScene(world, cam));
}
