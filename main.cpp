#include <iostream>
#include "Public/coreinterface.h"
#include "Public/ui.h"
#include "Framework/world.h"
#include "Framework/Rendering/scenebuilder.h"
#include "Framework/Loaders/meshloader.h"
#include "Framework/Loaders/programloader.h"
#include "Logger/logger.h"
#include "Framework/texturebuilder.h"
#include "resources_helper.h"
#include "custom_hints.h"
#include "step_builder.h"
#include "Math/mathhelpers.h"
#include "Graphics/graphics_events.h"
#include <algorithm>
#include <SDL2/SDL_scancode.h>

const auto logger = Logger::GetLogger("main");

float aspectFactor;
bool stop = false;
Camera cam;
Vec3 lookDir = {0, -1, 1};
Vec3 camPos = {500, 100, 500};
std::vector<std::reference_wrapper<Shader>> prog2;
std::vector<std::reference_wrapper<Shader>> prog;
std::vector<std::reference_wrapper<Shader>> skyboxProg;
std::vector<std::reference_wrapper<Shader>> hdrProg;
std::vector<std::reference_wrapper<Shader>> pbrProg;
std::vector<std::reference_wrapper<Shader>> normalsProg;
std::vector<std::reference_wrapper<Shader>> hdrIblProg;
std::vector<std::reference_wrapper<Shader>> hdrIblConvProg;
std::vector<std::reference_wrapper<Shader>> hdrIblPrefProg;
std::vector<std::reference_wrapper<Shader>> brdfLutProg;
std::vector<std::reference_wrapper<Shader>> texView;
std::vector<std::reference_wrapper<Shader>> testProg;
Light* light1;
Texture hdrMap;
Texture cMap;
float windowSizeX = 1280.0f, windowSizeY = 720.0f;
Camera* cam1;
Camera* cam2;
Camera* cam3;
World world;
World pistolWorld;
World sofaWorld;
World abstractWorld;

void updateViewport(WindowEvent* e, Renderer* renderer);
void closeApp(void*, CoreInterface* core);
void shiftKeyboard(KeyboardEvent* event, World& world);
void mouseRotate(MouseEvent* event, DrawableWorldObject& obj);
void mouseCamera(MouseEvent* event, Camera& cam);
void printFrameTime(FrameInfoEvent* event);

std::pair<SceneBuilder, SceneBuilder> initSceneBuilders(World& world);
void buildPistolWorld(ResourcesHelper& helper, StorageApi& api);
void pistolWorldAnimation(World& world);

int main()
{
    CoreInterface engine;
    Action<WindowEvent*> endGame{std::bind(closeApp, std::placeholders::_1, &engine)};
    aspectFactor = windowSizeX / windowSizeY;
    engine.Start();
    engine.getEventHandler().setListener<WindowEvent>(endGame, [](EventInterface* e) { return e->GetHint() == integral(WindowData::Type::Close); });
    engine.getEventHandler().setListener<KeyboardEvent>(endGame, [](EventInterface* e) { return e->GetHint() == SDL_SCANCODE_Q; });
    //engine.getEventHandler().setListener<FrameInfoEvent>(makeAction(std::function<void(FrameInfoEvent*)>{printFrameTime}));

    Renderer& renderer = engine.Video()->GetRenderer();
    Renderer& backupRenderer = engine.Video()->GetRenderer();
    Action<WindowEvent*> setViewport{std::bind(updateViewport, std::placeholders::_1, &renderer)};
    engine.getEventHandler().setListener<WindowEvent>(setViewport, [](EventInterface* e) { return e->GetHint() == integral(WindowData::Type::Resize); });
    Window window = engine.Video()->CreateWindow("Hello", windowSizeX, windowSizeY);
    renderer.SetWindow(window);
    UiLayer ui(engine.getCore(), window);
    ProgramLoader programLoader{*engine.Video()};
    prog = programLoader.Load("shadowmap_vertex_shader.glsl", "shadowmap_fragment_shader.glsl", "shadowmap_geometry_shader.glsl");
    prog2 = programLoader.Load("cubemap_vertex_shader.glsl", "cubemap_fragment_shader.glsl", "cubemap_geometry_shader.glsl");
    skyboxProg = programLoader.Load("skybox_vertex_shader.glsl", "skybox_fragment_shader.glsl");
    hdrProg = programLoader.Load("hdr_vertex_shader.glsl", "hdr_fragment_shader.glsl");
    pbrProg = programLoader.Load("pbr_vertex_shader.glsl", "pbr_fragment_shader.glsl");
    normalsProg = programLoader.Load("normals_vertex_shader.glsl", "normals_fragment_shader.glsl", "normals_geometry_shader.glsl");
    hdrIblProg = programLoader.Load("ibl_cubemap_vertex_shader.glsl", "ibl_cubemap_fragment_shader.glsl", "ibl_cubemap_geometry_shader.glsl");
    hdrIblConvProg = programLoader.Load("ibl_cubemap_vertex_shader.glsl", "ibl_cubemap_convolution_fragment_shader.glsl", "ibl_cubemap_geometry_shader.glsl");
    hdrIblPrefProg = programLoader.Load("ibl_cubemap_vertex_shader.glsl", "ibl_cubemap_prefilter_fragment_shader.glsl", "ibl_cubemap_geometry_shader.glsl");
    brdfLutProg = programLoader.Load("hdr_vertex_shader.glsl", "brdf_lut_fragment_shader.glsl");
    texView = programLoader.Load("hdr_vertex_shader.glsl", "texture_viewer.glsl");
    testProg = programLoader.Load("VertexShader.glsl", "FragmentShader.glsl");

    ResourcesHelper helper;

    helper.Init(engine.GetStorage());
    TextureLoader textureLoader{engine.GetStorage()};

    hdrMap = textureLoader.Load("lakeR.hdr", true);
    cMap = textureLoader.LoadCubemap({"negx.jpg", "posx.jpg", "negy.jpg", "posy.jpg", "negz.jpg", "posz.jpg"});
    Mesh plane = Shapes::Plane(1000, 1000, engine.GetStorage(), "floor");

    Action<MouseEvent*> rotate{std::bind(mouseCamera, std::placeholders::_1, std::ref(cam))};
    //Action<MouseEvent*> rotate(mouseRotate, std::placeholders::_1, std::ref(obj));
    Action<KeyboardEvent*> move{std::bind(shiftKeyboard, std::placeholders::_1, std::ref(world))};

    engine.getEventHandler().setListener<KeyboardEvent>(move);
    //engine.getEventHandler().setListener<MouseEvent>(rotate, [](EventInterface* e) { return e->GetHint() == integral(MouseData::Type::Motion); });
    engine.getEventHandler().setListener<MouseEvent>(rotate, [](EventInterface* e) { return e->GetHint() == integral(MouseData::Type::Motion); });

    buildPistolWorld(helper, engine.GetStorage());
    std::pair<SceneBuilder, SceneBuilder> builders1 = initSceneBuilders(pistolWorld);


    renderer.Draw(builders1.first.BuildScene(pistolWorld, *cam1));
    int frames = 0;
    while (!stop)
    {
        pistolWorldAnimation(pistolWorld);
        renderer.Draw(builders1.second.BuildScene(pistolWorld, *cam1));
        frames++;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return engine.WaitEnd();
}

void buildPistolWorld(ResourcesHelper& helper, StorageApi& api)
{
    Mesh plane = Shapes::Plane(1000, 1000, api, "floor1");

    cam1 = &pistolWorld.AddCamera({0, 0, 0});
    light1 = &pistolWorld.AddPointLight({500, 50, 500}, {100, 100, 100});
    pistolWorld.AddObject(plane, {0, 0, 0}, helper.GetMaterial(Materials::WoodenFloor)).SetTiling(8);
    DrawableWorldObject& obj = pistolWorld.AddObject(helper.GetMesh(Meshes::Pistol), {400, 50, 400});
    DrawableWorldObject& s = pistolWorld.AddObject(helper.GetMesh(Meshes::Cube), {0, 0, 0});
    s.SetHints({asObjectHint(CustomObjectHints::Skybox)});
    pistolWorld.AddObject(Shapes::Rectangle(api), {0, 0, 0}).SetHints({asObjectHint(CustomObjectHints::FullScreenQuad)});
    obj.SetScale(1);
    obj.Rotate(270.0f, {1, 0, 0});

    cam1->SetAspectRatio(aspectFactor);
    cam1->SetNearPlane(0.1f);
    cam1->SetFarPlane(1000.f);
    cam1->LookAt({500, 50, 500});
}

void pistolWorldAnimation(World& world)
{
    static float angle = 0;
    Light& light = world.GetPointLights().front();

    float radius = 150;
    float angleRad = degToRad(angle);
    Vec3 position = {radius * std::cos(angleRad) + 400, 50, radius * std::sin(angleRad) + 400} ;
    cam1->SetPosition(position);
    cam1->LookAt({400, 50, 400});
    Vec3 lightPosition = {radius * std::cos(-angleRad) + 400, 100, radius * std::sin(-angleRad) + 400};
    light.SetPosition(lightPosition);
    angle += 0.1f;
}

void printFrameTime(FrameInfoEvent* event)
{
    logger.LogDebug("Frame time: " + std::to_string(event->getPayload().frameTime));
}

void closeApp(void*, CoreInterface* core)
{
    logger.LogInfo("The end is coming");
    stop = true;
    core->Stop();
}

void updateViewport(WindowEvent* e, Renderer* renderer)
{
    logger.LogDebug("Viewport updated");
    aspectFactor = (float)e->getPayload().coordinates[0] / e->getPayload().coordinates[1];
    logger.LogDebug(std::to_string(aspectFactor));
    windowSizeX = e->getPayload().coordinates[0];
    windowSizeY = e->getPayload().coordinates[1];
    renderer->SetViewport(e->getPayload().coordinates[0], e->getPayload().coordinates[1]);
}

void shiftKeyboard(KeyboardEvent* event, World& world)
{
    float deg = 5.0f;
    static Vec3 wOffset = Vec3(0.0f);
    Vec3 lookDir = cam.GetLookDirection();

    Mat3 yRotate = rotationMatrix({0, 1, 0}, deg);
    Mat3 nyRotate = rotationMatrix({0, 1, 0}, -deg);
    Mat3 vRotate = rotationMatrix(glm::cross(Vec3{0, 1, 0}, lookDir), deg);
    Mat3 nvRotate = rotationMatrix(glm::cross(Vec3{0, 1, 0}, lookDir), -deg);

    if (light1 != nullptr)
    {
        Vec3 lightPos = light1->GetPosition();
        if (event->getPayload().scancode == SDL_SCANCODE_RIGHT)
            lightPos.x += 1;
        if (event->getPayload().scancode == SDL_SCANCODE_LEFT)
            lightPos.x -= 1;
        if (event->getPayload().scancode == SDL_SCANCODE_UP)
            lightPos.y += 1;
        if (event->getPayload().scancode == SDL_SCANCODE_DOWN)
            lightPos.y -= 1;
        light1->SetPosition(lightPos);
    }

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
    Vec3 right = glm::cross(lookDir, {0, 1, 0});
    if (event->getPayload().scancode == SDL_SCANCODE_W)
        camPos += lookDir;
    if (event->getPayload().scancode == SDL_SCANCODE_S)
        camPos -= lookDir;
    if (event->getPayload().scancode == SDL_SCANCODE_A)
        camPos -= right;
    if (event->getPayload().scancode == SDL_SCANCODE_D)
        camPos += right;

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

void mouseCamera(MouseEvent *event, Camera& cam)
{
    Vec3 lookDir = cam.GetLookDirection();

    Mat3 yRotate = rotationMatrix({0, 1, 0}, -event->getPayload().relativeCoordinates[0]);
    Mat3 nvRotate = rotationMatrix(glm::cross(Vec3{0, 1, 0}, lookDir), event->getPayload().relativeCoordinates[1]);

    lookDir = nvRotate * yRotate * lookDir;
    cam.SetLookDirection(lookDir);
}

std::pair<SceneBuilder, SceneBuilder> initSceneBuilders(World& world)
{
    SceneBuilder prepare;
    SceneBuilder main;
    StepBuilder builder;

    auto env = builder.Build(StepNames::HdrMapBuilder, world, hdrIblProg, {}, {{hdrMap}, {}});
    prepare.AddStep(env, asObjectHint(CustomObjectHints::Skybox));
    auto diffMap = builder.Build(StepNames::DiffuseMapBuilder, world, hdrIblConvProg, {env});
    prepare.AddStep(diffMap, asObjectHint(CustomObjectHints::Skybox));
    auto specMap = builder.Build(StepNames::SpecularMapBuilder, world, hdrIblPrefProg, {env});
    prepare.AddStep(specMap, asObjectHint(CustomObjectHints::Skybox));
    auto brdfMap = builder.Build(StepNames::BrdfBuilder, world, brdfLutProg, {});
    prepare.AddStep(brdfMap, asObjectHint(CustomObjectHints::FullScreenQuad));

    auto pointShadowMap = builder.Build(StepNames::PointShadows, world, prog2, {});
    main.AddStep(pointShadowMap);
    auto globalShadowMap = builder.Build(StepNames::GlobalShadows, world, prog, {});
    main.AddStep(globalShadowMap);
    auto pbrRender = builder.Build(StepNames::PbrRenderer, world, pbrProg, {diffMap, specMap, brdfMap, pointShadowMap, globalShadowMap}, {{}, {windowSizeX, windowSizeY}});
    main.AddStep(pbrRender);
    auto skybox = builder.Build(StepNames::SkyboxRenderer, world, skyboxProg, {env}, {{}, {}});
    main.AddStep(skybox, asObjectHint(CustomObjectHints::Skybox));
    auto result = builder.Build(StepNames::TextureViewer, world, hdrProg, {pbrRender}, {{}, {}});
    main.AddStep(result, asObjectHint(CustomObjectHints::FullScreenQuad));
//    auto r2 = builder.Build(StepNames::HdrMapBuilder, world, testProg, {}, {{hdrMap}, {}});
//    r2.renderStep.targets.clear();
//    main.AddStep(r2, asObjectHint(CustomObjectHints::Skybox));
    return {prepare, main};
}
