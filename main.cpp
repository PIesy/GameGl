#include <iostream>
#include "Public/coreinterface.h"
#include "Public/ui.h"
#include "Tests/tests.h"
#include "Framework/world.h"
#include "Framework/scenebuilder.h"
#include "Framework/Loaders/meshloader.h"
#include "Framework/Loaders/programloader.h"
#include "Logger/logger.h"

float aspectFactor;
bool stop = false;
Camera cam;
Camera lightCam;
Camera skyboxCam;
Vec3 lookDir = {0, -1, 1};
Vec3 camPos = {500, 500, 500};
Program* prog2;
Program* prog;
Program* skyboxProg;
Program* hdrProg;
Program* pbrProg;
Program* normalsProg;
Texture cubeMap;
float windowSizeX = 1000.0f, windowSizeY = 600.0f;

void updateViewport(WindowEvent* e, Renderer* renderer);
void closeApp(void*, CoreInterface* core);
void shiftKeyboard(KeyboardEvent* event, World& world);
void mouseRotate(MouseEvent* event, DrawableWorldObject& obj);

Program& prepareProgram(CoreInterface* engine);
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

    MeshLoader loader{engine.GetStorage()};
    TextureLoader textureLoader{engine.GetStorage()};

    cubeMap = textureLoader.LoadCubemap({"posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg"});
    auto meshes = loader.Load("sphere.blend");
    Mesh plane = Shapes::Plane(1000, 1000, engine.GetStorage(), "floor");
    Mesh skybox = loader.Load("cube.obj")[0];

    World world;
    plane.AddTexture(textureLoader.Load("snowS.jpg"));

    cam = world.addCamera(camPos);
    cam.SetAspectRatio(aspectFactor);
    cam.SetNearPlane(1.0f);
    cam.SetFarPlane(1700.f);
    cam.SetLookDirection(lookDir);

    lightCam = world.addCamera({450, 410, 450});
    lightCam.SetAspectRatio(1);
    lightCam.SetNearPlane(1.0f);
    lightCam.SetFarPlane(100.0f);
    lightCam.SetLookDirection({0, -1, 0});

    skyboxCam = world.addCamera({0, 0, 0});
    skyboxCam.SetAspectRatio(aspectFactor);
    skyboxCam.SetNearPlane(0.1f);
    skyboxCam.SetFarPlane(2.0f);
    skyboxCam.SetLookDirection(lookDir);

    DrawableWorldObject& obj = world.AddObject(meshes[0], {400, 420, 400});
    obj.SetScale(10.0);
    world.AddObject(plane, {0, 400, 0});
    DrawableWorldObject& s = world.AddObject(skybox, {0, 0, 0});
    s.SetRenderFlags(RenderFlags::SkyBox);

    world.AddObject(Shapes::Rectangle(engine.GetStorage()), {0, 0, 0}).SetRenderFlags(RenderFlags::FullScreenQuad);

    drawMesh2(world, renderer);

    Action<MouseEvent*> rotate(mouseRotate, std::placeholders::_1, std::ref(obj));
    Action<KeyboardEvent*> move(shiftKeyboard, std::placeholders::_1, std::ref(world));

    engine.getEventHandler().setListener<KeyboardEvent>(move);
    engine.getEventHandler().setListener<MouseEvent>(rotate, [](EventInterface* e) { return e->GetHint() == integral(MouseData::Type::Motion); });

    while (!stop)
    {
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
    skyboxCam.SetLookDirection(lookDir);
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

void drawMesh2(World& world, Renderer& renderer)
{
    SceneBuilder builder;
    RenderStep step1;
    RenderStep step2;
    RenderStep step2_5;
    RenderStep step3;
    RenderStep step4;
    TextureInfo textureInfo;
    TextureParameters textureParameters;
    Texture texture, texture2;

    cubeMap.parameters.sampling = TextureSampling::Linear;
    cubeMap.parameters.anisotropicFiltering = 16;
    step3.genericTextures = {cubeMap};
    step3.attributes = {RenderingAttribute::DepthTest};
    step3.prog = skyboxProg;
    step3.postConfig = [=](Program& p)
    {
        p.SetUniform(Vec3{0, 0, 0}, "worldCenter");
        p.SetUniform(skyboxCam.GetPerspectiveMatrix(), "perspective");
        p.SetUniform(skyboxCam.GetCameraMatrix(), "WtoCMatrix");
    };

    textureInfo.width = 2048;
    textureInfo.height = 2048;
    textureInfo.type = TextureType::Cubemap;
    textureInfo.target = TextureBindpoint::Depth;
    textureInfo.pixelFormat = TexturePixelFormat::Float24;
    textureInfo.name = "shadow";
    textureParameters.wrapping = TextureWrapping::Clamp;
    textureParameters.sampling = TextureSampling::Linear;
    texture.info = textureInfo;
    texture.parameters = textureParameters;
    step1.targets.push_back(RenderTarget(texture));
    step1.prog = prog2;

    std::vector<Mat4> cameraMatrices;

    cameraMatrices.push_back(lightCam.GetCameraMatrix({1.0, 0, 0}, {0, -1.0, 0}));
    cameraMatrices.push_back(lightCam.GetCameraMatrix({-1.0, 0, 0}, {0, -1.0, 0}));
    cameraMatrices.push_back(lightCam.GetCameraMatrix({0, 1.0, 0}, {0, 0, 1.0}));
    cameraMatrices.push_back(lightCam.GetCameraMatrix({0, -1.0, 0}, {0, 0, -1.0}));
    cameraMatrices.push_back(lightCam.GetCameraMatrix({0, 0, 1.0}, {0, -1.0, 0}));
    cameraMatrices.push_back(lightCam.GetCameraMatrix({0, 0, -1.0}, {0, -1.0, 0}));

    step1.postConfig = [=](Program& p)
    {
        p.SetUniform(cameraMatrices[0], "projections", 6);
        p.SetUniform(lightCam.GetPerspectiveMatrix(), "perspective");
        p.SetUniform(lightCam.GetCameraMatrix(), "camView");
        p.SetUniform(lightCam.GetNearPlane(), "nearPlane");
        p.SetUniform(lightCam.GetFarPlane(), "farPlane");
        p.SetUniform(lightCam.GetPosition(), "lightPosition");
    };


    Texture hdrTexture;
    hdrTexture.info.name = "colorBuffHdr";
    hdrTexture.info.pixelFormat = TexturePixelFormat::Float32;
    hdrTexture.info.channels = 4;
    hdrTexture.info.width = windowSizeX;
    hdrTexture.info.height = windowSizeY;

    step2.targets.push_back(RenderTarget(hdrTexture));
    step2.prog = pbrProg;
    step2.genericTextures = {texture};
    step2.postConfig = [=](Program& p)
    {
        p.SetUniform(lightCam.GetPerspectiveMatrix(), "lightMatrix");
        p.SetUniform(lightCam.GetPosition(), "lightPosition");
        p.SetUniform(Vec3(150.0f, 150.0f, 150.0f), "lightColor");
        p.SetUniform(lightCam.GetNearPlane(), "nearPlane");
        p.SetUniform(cam.GetPosition(), "camPos");
        p.SetUniform(lightCam.GetFarPlane(), "farPlane");
    };

    step2_5.attributes = {RenderingAttribute::DepthTest};
    step2_5.frameBufferProperties.frameBufferType = FrameBufferProperties::REUSE_FROM_STEP;
    step2_5.frameBufferProperties.stepId = 1;
    step2_5.prog = normalsProg;

    step3.frameBufferProperties.frameBufferType = FrameBufferProperties::REUSE_FROM_STEP;
    step3.frameBufferProperties.stepId = 1;

    step4.genericTextures = {hdrTexture};
    step4.prog = hdrProg;

    builder.AddStep(step1);
    builder.AddStep(step2);
    //builder.AddStep(step2_5);
    builder.AddStep(step3, RenderFlags::SkyBox);
    builder.AddStep(step4, RenderFlags::FullScreenQuad);

    renderer.Draw(builder.BuildScene(world, cam));
}
