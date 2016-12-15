#include <iostream>
#include "Public/coreinterface.h"
#include "Public/ui.h"
#include "Logger/logger.h"
#include "Tests/tests.h"
#include "Public/shaderreader.h"
#include "Public/meshreader3ds.h"
#include "Public/mtlreader.h"
#include "Public/meshreaderobj.h"
#include <functional>
#include <memory>
#include <thread>
#include <chrono>
#include "Framework/world.h"
#include "Framework/scenebuilder.h"
#include "Framework/Loaders/meshloader.h"
#include <stb_image.h>

Mat4 pers;
Mat4 MtoWMatrix;
Mat4 WtoCMatrix;
float aspectFactor;
glm::fquat rot = rotation;
glm::fquat look_rot = rotation;
bool stop = false;
bool zoomRotate = false;
float z_offset = 0;
Vec2 hello = {5, 5};
Camera cam;
Camera lightCam;
Vec3 lookDir = {1, 0, 1};
Vec3 camPos = {10, 10, 0};
Program* prog2;
Program* prog;

void updateViewport(WindowEvent* e, Renderer* renderer);
void closeApp(void*, CoreInterface* core);
void drawTriangle(CoreInterface* engine, UiLayer* ui, Renderer& renderer);
void test();
void switchPerspective(void*,Program* prog);
void shiftKeyboard(KeyboardEvent* event, World& world);
void zoom(KeyboardEvent* event, Program* prog);
void cubeSwitch(void*, GraphicsObject* cube, GraphicsObject* mesh, Renderer* renderer);
void mouseRotate(MouseEvent* event, DrawableWorldObject& obj);

void bindEvents(CoreInterface* engine, UiLayer* ui, Program& program, Program& prog2);
Program& prepareProgram(CoreInterface* engine);
void drawMesh(std::vector<GraphicsObject>& mesh, Renderer& renderer);
void drawMesh2(World& world, Renderer& renderer);
std::vector<GraphicsObject> getMesh(MeshReaderObj& meshReader, MtlReader& mtlReader, const std::string& meshPath, const std::string& mtlPath, CoreInterface* engine);
void rotateAboutZandZoom(Program& prog, bool reset, std::vector<GraphicsObject>& objs);
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
    bindEvents(&engine, &ui, *prog, *prog2);

    std::string basePath = "/home/anton/prog/fffrog3d_last/obj/";
    std::string fullPath = basePath + "untitled2_000";
    MeshLoader loader;

    auto meshes = loader.Load("test_2.obj");
    GraphicsObject plane = Shapes::Plane(1000, 1000);
    GraphicsObject wallPlane = Shapes::Plane(1000, 1000, {0, 0, 1.0f, 1.0f});
    GraphicsObject lightBox = Shapes::Box();
    plane.setAttribute("name", "floor");
    wallPlane.setAttribute("name", "wall");
    meshes[0].setAttribute("name", "sofa");
    lightBox.setAttribute("name", "light");
    lightBox.Scale(20, 20, 20);
    glm::fquat wallRot = rotation;
    rotateQuat(wallRot, 270, {1, 0, 0});
    World world;
    int x, y, n;
    void* data = stbi_load("avatar.png", &x, &y, &n, 0);
    Texture tex;
    tex.data = data;
    tex.height = y;
    tex.width = x;
    tex.size = x * y * n;
    void* data2 = stbi_load("frontend-large.jpg", &x, &y, &n, 0);
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
    world.addObject(plane, {0, 0, 0});
    DrawableWorldObject& wall = world.addObject(wallPlane, {0, 0, 0});
    //world.addObject(lightBox, {50, 50, 50});
    wall.setRotation(glm::mat4_cast(wallRot));
    wall.getObject().setTexture(tex);

    drawMesh2(world, renderer);

    Action<MouseEvent*> rotate(mouseRotate, std::placeholders::_1, std::ref(obj));
    Action<KeyboardEvent*> move(shiftKeyboard, std::placeholders::_1, std::ref(world));

    engine.getEventHandler().setListener<KeyboardEvent>(move);
    engine.getEventHandler().setListener<MouseEvent>(rotate, [](EventInterface* e) { return e->getHint() == integral(MouseData::Type::Motion); });

    std::vector<std::vector<GraphicsObject>> objects;
    std::string finalPath;
    for (int j = 1; j < 201; j++)
    {
        if (j < 10)
            finalPath = fullPath + "00" + std::to_string(j);
        if (j >= 10 && j < 100)
            finalPath = fullPath + "0" + std::to_string(j);
        if (j >= 100)
            finalPath = fullPath + std::to_string(j);
        finalPath += ".obj";
        objects.push_back(loader.Load(finalPath));
        int i = 0;
        for (GraphicsObject& m : objects.back())
        {
            m.setAttribute("name", std::to_string(i++));
            m.Scale(2000, 2000, 2000);
        }
    }

    while (!stop)
    {
        for (int j = 0; j < 200; j++)
        {
            if (j != 0)
            {
                for (GraphicsObject& m : objects[j - 1])
                {
                    DrawableWorldObject o;
                    o.setObject(m);
                    world.removeObject(o);
                }
            }
            for (GraphicsObject& m : objects[j])
                world.addObject(m, {50, 5, 15});
            drawMesh2(world, renderer);
            std::this_thread::sleep_for(std::chrono::milliseconds(13));
        }
    }

    return engine.WaitEnd();
}

void closeApp(void*, CoreInterface* core)
{
    Logger::Log("The end is comming");
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

void switchPerspective(void *, Program *prog)
{
    static bool persp = true;
    if(persp)
    {
        persp = false;
        prog->SetPerspective(identity);
        Logger::Log("Perspective off");
    }
    else
    {
        persp = true;
        prog->SetPerspective(pers);
        Logger::Log("Perspective on");
    }
}

void zoom(KeyboardEvent *event, Program *prog)
{
    if(event->getPayload().scancode == SDL_SCANCODE_EQUALS)
    {
        pers[0][0] += 0.01 / aspectFactor;
        pers[1][1] += 0.01;
    }
    else
    {
        pers[0][0] -= 0.01 / aspectFactor;
        pers[1][1] -= 0.01;
    }
    prog->SetPerspective(pers);
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
    if(event->getPayload().state == true)
        rotateQuat(rot, degree, axis);
    obj.setRotation(glm::mat4_cast(rot));
}

void bindEvents(CoreInterface* engine, UiLayer* ui, Program& program, Program& prog2)
{
    Action<MouseEvent*> endGame(switchPerspective, std::placeholders::_1, &program);

    Action<KeyboardEvent*> zoomObj(zoom, std::placeholders::_1, &program);

    engine->getEventHandler().setListener<KeyboardEvent>(zoomObj, [](EventInterface* e) { return e->getHint() == SDL_SCANCODE_EQUALS ||
                e->getHint() == SDL_SCANCODE_MINUS; });
    Button* box = new Button(200, 50);

    ui->AddElement(box);
    box->setPosition(400, 275);
    box->setAction(Events::onClick, endGame);
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
    program.SetLight(light);
    program.SetIntensity(1);
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
    program.SetLight(light);
    program.SetIntensity(1);
    return program;
}

std::vector<GraphicsObject> getMesh(MeshReaderObj& meshReader, MtlReader& mtlReader, const std::string& meshPath, const std::string& mtlPath, CoreInterface* engine)
{
    engine->Resources().ReadByTemplate(meshPath, meshReader);
    engine->Resources().ReadByTemplate(mtlPath, mtlReader);
    std::vector<GraphicsObject> meshes = meshReader.getResult();
    std::vector<Material> materials = mtlReader.getResult();

    for (GraphicsObject& mesh : meshes)
    {
        for (Material& mat : materials)
        {
            if (mat.name == mesh.getAttribute("material"))
            {
                for (Vertex& vertex : mesh.data().vertices)
                    vertex.color = mat.diffuseColor;
            }
        }
    }
    return meshes;
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
