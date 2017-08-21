#include <iostream>
#include "Public/coreinterface.h"
#include "Public/ui.h"
#include "Tests/tests.h"
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
#include <algorithm>

float aspectFactor;
bool stop = false;
Camera cam;
Vec3 lookDir = {0, -1, 1};
Vec3 camPos = {500, 100, 500};
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
Program* texView;
Program* testProg;
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
void mouseCamera(MouseEvent *event, Camera& cam);

std::pair<SceneBuilder, SceneBuilder> initSceneBuilders(World& world);
void buildPistolWorld(ResourcesHelper& helper, StorageApi& api);
void buildSofaWorld(ResourcesHelper& helper, StorageApi& api);
void pistolWorldAnimation(World& world);
void buildMaterialWorld(ResourcesHelper& helper, StorageApi& api);

Program& prepareProgram2(CoreInterface* engine);

int main()
{
    CoreInterface engine;
    Renderer& renderer = engine.Video()->GetRenderer();
    Action<WindowEvent*> setViewport{std::bind(updateViewport, std::placeholders::_1, &renderer)};
    Action<WindowEvent*> endGame{std::bind(closeApp, std::placeholders::_1, &engine)};
    aspectFactor = windowSizeX / windowSizeY;
    engine.Start();
    eventsHandlerTest();
    engine.getEventHandler().setListener<WindowEvent>(endGame, [](EventInterface* e) { return e->GetHint() == integral(WindowData::Type::Close); });
    engine.getEventHandler().setListener<WindowEvent>(setViewport, [](EventInterface* e) { return e->GetHint() == integral(WindowData::Type::Resize); });
    engine.getEventHandler().setListener<KeyboardEvent>(endGame, [](EventInterface* e)
    {
        return e->GetHint() == SDL_SCANCODE_Q;
    });
    Window window = engine.Video()->CreateWindow("Hello", windowSizeX, windowSizeY);
    renderer.SetWindow(window);
    UiLayer ui(engine.getCore(), window);
    ProgramLoader programLoader{*engine.Video()};
    prog = &programLoader.Load("shadowmap_vertex_shader.glsl", "shadowmap_fragment_shader.glsl", "shadowmap_geometry_shader.glsl");
    prog2 = &prepareProgram2(&engine);
    skyboxProg = &programLoader.Load("skybox_vertex_shader.glsl", "skybox_fragment_shader.glsl");
    hdrProg = &programLoader.Load("hdr_vertex_shader.glsl", "hdr_fragment_shader.glsl");
    pbrProg = &programLoader.Load("pbr_vertex_shader.glsl", "pbr_fragment_shader.glsl");
    normalsProg = &programLoader.Load("normals_vertex_shader.glsl", "normals_fragment_shader.glsl", "normals_geometry_shader.glsl");
    hdrIblProg = &programLoader.Load("ibl_cubemap_vertex_shader.glsl", "ibl_cubemap_fragment_shader.glsl", "ibl_cubemap_geometry_shader.glsl");
    hdrIblConvProg = &programLoader.Load("ibl_cubemap_vertex_shader.glsl", "ibl_cubemap_convolution_fragment_shader.glsl", "ibl_cubemap_geometry_shader.glsl");
    hdrIblPrefProg = &programLoader.Load("ibl_cubemap_vertex_shader.glsl", "ibl_cubemap_prefilter_fragment_shader.glsl", "ibl_cubemap_geometry_shader.glsl");
    brdfLutProg = &programLoader.Load("hdr_vertex_shader.glsl", "brdf_lut_fragment_shader.glsl");
    texView = &programLoader.Load("hdr_vertex_shader.glsl", "texture_viewer.glsl");
    testProg = &programLoader.Load("VertexShader.glsl", "FragmentShader.glsl");

    ResourcesHelper helper;

    helper.Init(engine.GetStorage());
    TextureLoader textureLoader{engine.GetStorage()};

    hdrMap = textureLoader.Load("lakeR.hdr", true);
    cMap = textureLoader.LoadCubemap({"negx.jpg", "posx.jpg", "negy.jpg", "posy.jpg", "negz.jpg", "posz.jpg"});
    Mesh plane = Shapes::Plane(1000, 1000, engine.GetStorage(), "floor");

    cam = world.AddCamera(camPos);
    cam.SetAspectRatio(aspectFactor);
    cam.SetNearPlane(1.0f);
    cam.SetFarPlane(1000.f);
    cam.SetLookDirection(lookDir);

//    light1 = &world.AddPointLight({450, 50, 450}, {200, 200, 200});
//    world.AddLight({350, 410, 350}, {0, 300, 0});
//    world.AddLight({450, 440, 450}, {0, 0, 300});
//    world.AddLight({350, 440, 350}, {150, 150, 150});
    world.AddDirectionalLight({1, 1, 1}, {1, -1, 1});
//    world.AddDirectionalLight({100, 0, 0}, {0, -1, 0});

    auto& sofa = world.AddObject(helper.GetMesh(Meshes::Sofa), {600, 0, 600});
    sofa.SetScale(0.05);
    sofa.SetTiling(4);
    sofa.SetAoTiling(false);
    sofa.Rotate(-90, {1, 0, 0});
    world.AddObject(helper.GetMesh(Meshes::Table), {500, 0, 300}).SetScale(100);
    DrawableWorldObject& obj = world.AddObject(helper.GetMesh(Meshes::Pistol), {400, 20, 400});
    obj.SetScale(1);
    obj.Rotate(270.0f, {1, 0, 0});
    world.AddObject(plane, {0, 0, 0}, helper.GetMaterial(Materials::WoodenFloor)).SetTiling(8);
    auto& sphere = helper.GetMesh(Meshes::Sphere);
    world.AddObject(sphere, {500, 10, 500}, helper.GetMaterial(Materials::Gold)).SetScale(10);
    sphere[0].SetId("spherePlast");
    world.AddObject(sphere, {530, 10, 530}, helper.GetMaterial(Materials::Plastic)).SetScale(10);
    sphere[0].SetId("sphereIron");
    world.AddObject(sphere, {470, 10, 470}, helper.GetMaterial(Materials::Iron)).SetScale(10);
    DrawableWorldObject& s = world.AddObject(helper.GetMesh(Meshes::Cube), {0, 0, 0});
    s.SetHints({asObjectHint(CustomObjectHints::Skybox)});

    world.AddObject(Shapes::Rectangle(engine.GetStorage()), {0, 0, 0}).SetHints({asObjectHint(CustomObjectHints::FullScreenQuad)});

    Action<MouseEvent*> rotate{std::bind(mouseCamera, std::placeholders::_1, std::ref(cam))};
    //Action<MouseEvent*> rotate(mouseRotate, std::placeholders::_1, std::ref(obj));
    Action<KeyboardEvent*> move{std::bind(shiftKeyboard, std::placeholders::_1, std::ref(world))};

    engine.getEventHandler().setListener<KeyboardEvent>(move);
    //engine.getEventHandler().setListener<MouseEvent>(rotate, [](EventInterface* e) { return e->GetHint() == integral(MouseData::Type::Motion); });
    engine.getEventHandler().setListener<MouseEvent>(rotate, [](EventInterface* e) { return e->GetHint() == integral(MouseData::Type::Motion); });

    buildPistolWorld(helper, engine.GetStorage());
    buildSofaWorld(helper, engine.GetStorage());
    buildMaterialWorld(helper, engine.GetStorage());
    std::pair<SceneBuilder, SceneBuilder> builders1 = initSceneBuilders(pistolWorld);

    renderer.Draw(builders1.first.BuildScene(pistolWorld, *cam1));
    int frames = 0;
    while (!stop)
    {
        if (frames < 6000)
        {
            pistolWorldAnimation(pistolWorld);
            renderer.Draw(builders1.second.BuildScene(pistolWorld, *cam1));
        }
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

void buildSofaWorld(ResourcesHelper& helper, StorageApi& api)
{
    Mesh plane = Shapes::Plane(1000, 1000, api, "floor1");
    Mesh plane2 = Shapes::Plane(33, 33, api, "golden plate");

    cam2 = &sofaWorld.AddCamera({520, 50, 520});
    light1 = &sofaWorld.AddDirectionalLight({1, 1, 1}, {1, -1, 1});
    sofaWorld.AddObject(plane2, {465, 29, 483}, helper.GetMaterial(Materials::Gold)).SetTiling(2);
    sofaWorld.AddObject(plane, {0, 0, 0}, helper.GetMaterial(Materials::WoodenFloor)).SetTiling(8);
    sofaWorld.AddObject(helper.GetMesh(Meshes::Table), {500, 0, 500}).SetScale(75);
    auto& sofa = sofaWorld.AddObject(helper.GetMesh(Meshes::Sofa), {500, 0, 450});
    sofa.SetScale(0.05);
    sofa.SetTiling(4);
    sofa.SetAoTiling(false);
    sofa.Rotate(-90, {1, 0, 0});

    auto& sphere = helper.GetMesh(Meshes::Sphere);
    sofaWorld.AddObject(sphere, {470, 32, 500}, helper.GetMaterial(Materials::Gold)).SetScale(3);
    sofaWorld.AddObject(sphere, {495, 32, 495}, helper.GetMaterial(Materials::Plastic)).SetScale(3);
    sofaWorld.AddObject(sphere, {475, 32, 510}, helper.GetMaterial(Materials::Iron)).SetScale(3);

    DrawableWorldObject& s = sofaWorld.AddObject(helper.GetMesh(Meshes::Cube), {0, 0, 0});
    s.SetHints({asObjectHint(CustomObjectHints::Skybox)});
    sofaWorld.AddObject(Shapes::Rectangle(api), {0, 0, 0}).SetHints({asObjectHint(CustomObjectHints::FullScreenQuad)});

    cam2->SetAspectRatio(aspectFactor);
    cam2->SetNearPlane(1.0f);
    cam2->SetFarPlane(1000.f);
    cam2->SetLookDirection({-1, -0.7f, -1});
}

void buildMaterialWorld(ResourcesHelper& helper, StorageApi& api)
{
    auto& sphere = helper.GetMesh(Meshes::Sphere);
    abstractWorld.AddObject(sphere, {500, 500, 500}, helper.GetMaterial(Materials::Gold)).SetScale(10);

    DrawableWorldObject& s = abstractWorld.AddObject(helper.GetMesh(Meshes::Cube), {0, 0, 0});
    s.SetHints({asObjectHint(CustomObjectHints::Skybox)});
    abstractWorld.AddObject(Shapes::Rectangle(api), {0, 0, 0}).SetHints({asObjectHint(CustomObjectHints::FullScreenQuad)});

    cam3 = &abstractWorld.AddCamera({480, 500, 480});
    cam3->SetAspectRatio(aspectFactor);
    cam3->SetNearPlane(1.0f);
    cam3->SetFarPlane(1000.f);
    cam3->SetLookDirection({1, 0, 1});
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

Program& prepareProgram2(CoreInterface* engine)
{
    ProgramLoader loader{*engine->Video()};
    return loader.Load("cubemap_vertex_shader.glsl", "cubemap_fragment_shader.glsl", "cubemap_geometry_shader.glsl");
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
