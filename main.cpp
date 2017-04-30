#include <iostream>
#include "Public/coreinterface.h"
#include "Public/ui.h"
#include "Logger/logger.h"
#include "Tests/tests.h"
#include "Public/shaderreader.h"
#include "Framework/world.h"
#include "Framework/scenebuilder.h"
#include "Framework/Loaders/meshloader.h"

float aspectFactor;
bool stop = false;
Camera cam;
Camera lightCam;
Vec3 lookDir = {1, 0, 1};
Vec3 camPos = {10, 10, 0};
Program* prog2;
Program* prog;

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
    aspectFactor = 1000.0f / 600.0f;
    engine.Start();
    eventsHandlerTest();
    engine.getEventHandler().setListener<WindowEvent>(endGame, [](EventInterface* e) { return e->GetHint() == integral(WindowData::Type::Close); });
    engine.getEventHandler().setListener<WindowEvent>(setViewport, [](EventInterface* e) { return e->GetHint() == integral(WindowData::Type::Resize); });
    engine.getEventHandler().setListener<KeyboardEvent>(endGame, [](EventInterface* e) { return e->GetHint() == SDL_SCANCODE_Q; });
    Window window = engine.Video()->CreateWindow("Hello", 1000, 600);
    renderer.SetWindow(window);
    UiLayer ui(engine.getCore(), window);
    prog = &prepareProgram(&engine);
    prog2 = &prepareProgram2(&engine);

    MeshLoader loader{engine.GetStorage()};
    TextureLoader textureLoader{engine.GetStorage()};

    auto meshes = loader.Load("temple.3ds");
    Mesh plane = Shapes::Plane(1000, 1000, engine.GetStorage(), "floor");
    Mesh wallPlane = Shapes::Plane(1000, 1000, engine.GetStorage(), "wall", {0, 1.0f, 1.0f, 1.0f});
    World world;
    wallPlane.AddTexture(textureLoader.Load("landscapeS.jpg"));
    plane.AddTexture(textureLoader.Load("snowS.jpg"));

    cam = world.addCamera({0, 0, 0});
    cam.setAspectRatio(1000.0f / 600.0f);
    cam.setNearPlane(1.f);
    cam.setFarPlane(1000.f);
    cam.setLookDirection(lookDir);

    lightCam = world.addCamera({50, 100, 50});
    lightCam.setAspectRatio(1);
    lightCam.setNearPlane(0.1f);
    lightCam.setFarPlane(1000.0f);
    lightCam.setLookDirection({0.0f, -1.0f, 0.0f});

    DrawableWorldObject& obj = world.AddObject(meshes[0], {10, 0, 10});
    //DrawableWorldObject& obj2 = world.AddObject(meshes[1], {10, 0, 10});
    world.AddObject(plane, {0, 0, 0});
    DrawableWorldObject& wall = world.AddObject(wallPlane, {0, 0, 0});
    wall.Rotate(270, {1, 0, 0});

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
        camPos = {0, 10, 0};
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

    cam.setLookDirection(lookDir);
    cam.setPosition(camPos);
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
    ShaderReader reader;
    engine->Resources().ReadByTemplate("VertexShader.glsl", reader);
    std::string vertexsrc = reader.getResult();
    engine->Resources().ReadByTemplate("FragmentShader.glsl", reader);
    std::string fragsrc = reader.getResult();
    Shader& shader1 = engine->Video()->CreateShader(vertexsrc, ShaderType::VertexShader);
    Shader& shader2 = engine->Video()->CreateShader(fragsrc, ShaderType::FragmentShader);
    Program& program = engine->Video()->CreateProgram();

    program.Attach(shader1);
    program.Attach(shader2);
    program.Compile();
    Mat4 rot(1.0f);
    program.setUniform(rot, "rotation");
    return program;
}

Program& prepareProgram2(CoreInterface* engine)
{
    ShaderReader reader;
    engine->Resources().ReadByTemplate("vertexshadertex.glsl", reader);
    std::string vertexsrc = reader.getResult();
    engine->Resources().ReadByTemplate("fragmentshadertex.glsl", reader);
    std::string fragsrc = reader.getResult();
    Shader& shader1 = engine->Video()->CreateShader(vertexsrc, ShaderType::VertexShader);
    Shader& shader2 = engine->Video()->CreateShader(fragsrc, ShaderType::FragmentShader);
    Program& program = engine->Video()->CreateProgram();

    program.Attach(shader1);
    program.Attach(shader2);
    program.Compile();
    Mat4 rot(1.0f);
    program.setUniform(rot, "rotation");
    return program;
}

void drawMesh2(World& world, Renderer& renderer)
{
    SceneBuilder builder;
    RenderStep step1;
    RenderStep step2;
    TextureInfo textureInfo;
    TextureParameters textureParameters;
    Texture texture;

    textureInfo.width = 4096;
    textureInfo.height = 4096;
    textureInfo.type = TextureType::Tex2D;
    textureInfo.target = TextureBindpoint::Depth;
    textureInfo.pixelFormat = TexturePixelFormat::Float32;
    textureInfo.name = "shadow";

    textureParameters.wrapping = TextureWrapping::Clamp;
    textureParameters.sampling = TextureSampling::Nearest;
    texture.info = textureInfo;
    texture.parameters = textureParameters;
    step1.targets.push_back(RenderTarget(texture));
    step1.prog = prog2;
    step1.postConfig = [lightCam](Program& p)
    {
        p.setUniform(lightCam.GetCameraMatrix(), "WtoCMatrix");
        p.setUniform(lightCam.GetOrthographicMatrix(), "perspective");
    };

    step2.prog = prog;
    step2.genericTexture = texture;
    step2.postConfig = [lightCam](Program& p)
    {
        p.setUniform(lightCam.GetOrthographicMatrix() * lightCam.GetCameraMatrix(), "lightMatrix");
    };

    builder.AddStep(step1);
    builder.AddStep(step2);

    renderer.Draw(builder.BuildScene(world, cam));
}
