#include <iostream>
#include "Public/coreinterface.h"
#include "Public/ui.h"
#include "Logger/logger.h"
#include "Tests/tests.h"
#include "Public/shaderreader.h"
#include "Framework/world.h"
#include "Framework/scenebuilder.h"
#include "Framework/Loaders/meshloader.h"
#include <stb_image.h>

Mat4 pers;
float aspectFactor;
glm::fquat rot = rotation;
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
    pers = perspective;
    engine.Start();
    eventsHandlerTest();
    engine.getEventHandler().setListener<WindowEvent>(endGame, [](EventInterface* e) { return e->getHint() == integral(WindowData::Type::Close); });
    engine.getEventHandler().setListener<WindowEvent>(setViewport, [](EventInterface* e) { return e->getHint() == integral(WindowData::Type::Resize); });
    engine.getEventHandler().setListener<KeyboardEvent>(endGame, [](EventInterface* e) { return e->getHint() == SDL_SCANCODE_Q; });
    Window window = engine.Video()->CreateWindow("Hello", 1000, 600);
    pers[0][0] = 1.0 / aspectFactor;
    renderer.SetWindow(window);
    UiLayer ui(engine.getCore(), window);
    prog = &prepareProgram(&engine);
    prog2 = &prepareProgram2(&engine);

    MeshLoader loader;

    //std::vector<GraphicsObject> meshes = {Shapes::Box()};
    auto meshes = loader.Load("temple.3ds");
    GraphicsObject plane = Shapes::Plane(1000, 1000);
    GraphicsObject wallPlane = Shapes::Plane(1000, 1000, {0, 0, 1.0f, 1.0f});
    GraphicsObject lightBox = Shapes::Box();
    plane.setAttribute("name", "floor");
    wallPlane.setAttribute("name", "wall");
    meshes[0].setAttribute("name", "sofa");
    //meshes[0].Scale(20, 20, 20);
    lightBox.setAttribute("name", "light");
    lightBox.Scale(20, 20, 20);
    glm::fquat wallRot = rotation;
    rotateQuat(wallRot, 270, {1, 0, 0});
    World world;
    int x, y, n;
    void* data = stbi_load("wall.jpg", &x, &y, &n, 0);
    Texture tex;
    tex.data = data;
    tex.height = y;
    tex.width = x;
    tex.size = x * y * n;
    void* data2 = stbi_load("snow.jpg", &x, &y, &n, 0);
    Texture tex2;
    tex2.data = data2;
    tex2.height = y;
    tex2.width = x;
    tex2.size = x * y * n;

    cam = world.addCamera({0, 0, 0});
    cam.setAspectRatio(1000.0f / 600.0f);
    cam.setNearPlane(0.1f);
    cam.setFarPlane(1000);
    cam.setLookDirection(lookDir);

    lightCam = world.addCamera({50, 100, 50});
    lightCam.setAspectRatio(1000.0f / 600.0f);
    lightCam.setNearPlane(0.1f);
    lightCam.setFarPlane(1000.0f);
    lightCam.setLookDirection({-0.1, -1, -0.1});

    DrawableWorldObject& obj = world.addObject(meshes[0], {10, 0, 10});
    world.addObject(plane, {0, 0, 0}).getObject().setTexture(tex2);
    DrawableWorldObject& wall = world.addObject(wallPlane, {0, 0, 0});
    //world.addObject(lightBox, {50, 50, 50});
    wall.setRotation(glm::mat4_cast(wallRot));
    wall.getObject().setTexture(tex);

    drawMesh2(world, renderer);

    Action<MouseEvent*> rotate(mouseRotate, std::placeholders::_1, std::ref(obj));
    Action<KeyboardEvent*> move(shiftKeyboard, std::placeholders::_1, std::ref(world));

    engine.getEventHandler().setListener<KeyboardEvent>(move);
    engine.getEventHandler().setListener<MouseEvent>(rotate, [](EventInterface* e) { return e->getHint() == integral(MouseData::Type::Motion); });

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
    pers[0][0] = 1.0 / aspectFactor;
    Logger::Log(std::to_string(pers[0][0]));
    renderer->SetViewport(e->getPayload().coordinates[0], e->getPayload().coordinates[1]);
}

void shiftKeyboard(KeyboardEvent *event, World& world)
{
    float rad = degToRad(5.0f);
    float nrad = degToRad(355.0f);
    static Vec3 wOffset = Vec3(0.0f);

    Mat3 yRotate = {{cos(rad), 0, -sin(rad)}, {0, 1, 0}, {sin(rad), 0, cos(rad)}};

    Mat3 nyRotate = {{cos(nrad), 0, -sin(nrad)}, {0, 1, 0}, {sin(nrad), 0, cos(nrad)}};

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
        lookDir.y += 0.1f;
    if (event->getPayload().scancode == SDL_SCANCODE_DOWN)
        lookDir.y -= 0.1f;

    if (event->getPayload().scancode == SDL_SCANCODE_SPACE)
        camPos.y += 1;

    if (event->getPayload().scancode == SDL_SCANCODE_C)
        camPos.y -= 1;

    if (event->getPayload().scancode == SDL_SCANCODE_RETURN)
    {
        lookDir = {1, 0, 1};
        camPos = {0, 10, 0};
        rot = rotation;
    }
    lookDir = glm::normalize(lookDir);
    if (event->getPayload().scancode == SDL_SCANCODE_W)
    {
        camPos.x += 1 * lookDir.x;
        camPos.z += 1 * lookDir.z;
    }
    if (event->getPayload().scancode == SDL_SCANCODE_S)
    {
        camPos.x -= 1 * lookDir.x;
        camPos.z -= 1 * lookDir.z;
    }

    cam.setLookDirection(lookDir);
    cam.setPosition(camPos);
}

void mouseRotate(MouseEvent *event, DrawableWorldObject& obj)
{
    float degree = sqrt(pow(event->getPayload().relativeCoordinates[0], 2) + pow(event->getPayload().relativeCoordinates[1], 2));
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
    TextureParameters tex;
    Texture text;
    text.stepId = 0;
    text.textureId = 1;

    tex.width = 4096;
    tex.height = 4096;
    tex.type = TextureType::Depth;
    step1.target = TextureRenderTarget(tex);
    step1.prog = prog2;
    step1.postConfig = [=](Program& p)
    {
        p.setUniform(lightCam.GetCameraMatrix(), "WtoCMatrix");
        p.setUniform(lightCam.GetPerspectiveMatrix(), "perspective");
    };

    step2.prog = prog;
    step2.genericTexture = text;
    step2.postConfig = [=](Program& p)
    {
        p.setUniform(lightCam.GetPerspectiveMatrix() * lightCam.GetCameraMatrix(), "lightMatrix");
    };

    builder.AddStep(step1);
    builder.AddStep(step2);

    renderer.Draw(builder.BuildScene(world, cam));
}
