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

Mat4 pers;
float aspectFactor;
glm::fquat rot = rotation;
bool stop = false;
bool zoomRotate = false;
float z_offset = 0;
Vec2 hello = {5, 5};

void updateViewport(WindowEvent* e, Renderer* renderer);
void closeApp(void*, CoreInterface* core);
void drawTriangle(CoreInterface* engine, UiLayer* ui, Renderer& renderer);
void test();
void switchPerspective(void*,Program* prog);
void shiftKeyboard(KeyboardEvent* event, Program* prog);
void zoom(KeyboardEvent* event, Program* prog);
void cubeSwitch(void*, GraphicsObject* cube, GraphicsObject* mesh, Renderer* renderer);
void mouseRotate(MouseEvent* event, Program* prog);

void bindEvents(CoreInterface* engine, UiLayer* ui, Program& program);
Program& prepareProgram(CoreInterface* engine);
void drawMesh(std::vector<GraphicsObject>& mesh, Renderer& renderer, Program& prog);
std::vector<GraphicsObject> getMesh(MeshReaderObj& meshReader, MtlReader& mtlReader, const std::string& meshPath, const std::string& mtlPath, CoreInterface* engine);
void rotateAboutZandZoom(Program& prog, bool reset, std::vector<GraphicsObject>& objs);

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
    engine.getEventHandler().setListener<WindowEvent>(setViewport, [](EventInterface* e) { return e->getHint() == integral(WindowData::Type::Resize); });
    engine.getEventHandler().setListener<WindowEvent>(endGame, [](EventInterface* e) { return e->getHint() == integral(WindowData::Type::Close); });
    Window window = engine.Video()->CreateWindow("Hello", 1000, 600);
    pers[0][0] = 1.0 / aspectFactor;
    renderer.SetWindow(window);
    UiLayer ui(engine.getCore(), window);
    Program& prog = prepareProgram(&engine);
    bindEvents(&engine, &ui, prog);

    std::string basePath = "/home/anton/prog/frog_beta2/frog_000";
    std::string fullPath;
    int i = 1;
    MeshReaderObj meshReader;
    engine.Resources().ReadByTemplate("test_2.obj", meshReader);

    std::vector<GraphicsObject> meshes = meshReader.getResult();
    meshes[0].Scale(0.1, 0.1, 0.1);

    drawMesh(meshes, renderer, prog);
    UniformValue value;
    value.type = UniformTypes::FLOAT;
    value.value = &z_offset;
    prog.setUniform("z_offset", value);

//    std::vector<GraphicsObject> objects;
//    while (!stop)
//    {
//        MeshReaderObj meshReader;
//        MtlReader mtlReader;
//        if (i == 120)
//        {
//            i = 1;
//        }
//        if (i < 10)
//        {
//            fullPath = basePath + "00" + std::to_string(i);
//        }
//        else
//        {
//            if (i < 100)
//            {
//                fullPath = basePath + "0" + std::to_string(i);
//            }
//            else
//            {
//                if (i < 121)
//                {
//                    fullPath = basePath + std::to_string(i);
//                }
//            }
//        }
//        objects = getMesh(meshReader, mtlReader, fullPath + ".obj", fullPath + ".mtl", &engine);
//        //rotateAboutZandZoom(prog, !zoomRotate, objects);
//        if (i < 100)
//            objects.erase(objects.begin());
//        drawMesh(objects, renderer, prog);
//        std::this_thread::sleep_for(std::chrono::milliseconds(33));
//        i++;
//    }
    return engine.WaitEnd();
}

void rotateAboutZandZoom(Program& prog, bool reset, std::vector<GraphicsObject>& objs)
{
    static float scale = 1;
    static float offsetx = 0;
    if (!reset)
    {
        if (offsetx > -5.5)
            offsetx -= 0.05;
        scale = scale + 0.01;
        rotateQuat(rot, +2.0f, {0, 0, 1});
        for (GraphicsObject& obj : objs)
        {
            obj.Scale(scale, scale);
            obj.Move(offsetx);
        }
    }
    else
    {
        rot = rotation;
        scale = 1;
        offsetx = 0;
    }

    prog.SetRotation(glm::mat4_cast(rot));
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

void shiftKeyboard(KeyboardEvent *event, Program *prog)
{
    if (event->getPayload().scancode == SDL_SCANCODE_LEFT)
        rotateQuat(rot, -5.0f, {0, 1, 0});
    if (event->getPayload().scancode == SDL_SCANCODE_RIGHT)
        rotateQuat(rot, +5.0f, {0, 1, 0});
    if (event->getPayload().scancode == SDL_SCANCODE_UP)
        rotateQuat(rot, -5.0f, {1, 0, 0});
    if (event->getPayload().scancode == SDL_SCANCODE_DOWN)
        rotateQuat(rot, +5.0f, {1, 0, 0});
    if (event->getPayload().scancode == SDL_SCANCODE_SPACE)
        rot = rotation;
    if (event->getPayload().scancode == SDL_SCANCODE_RETURN)
        zoomRotate = !zoomRotate;
    prog->SetRotation(glm::mat4_cast(rot));
}

void cubeSwitch(void*,GraphicsObject *cube, GraphicsObject *mesh, Renderer* renderer)
{
    static bool isCube = false;
    if(isCube)
    {
        isCube = false;
        Scene scene;
        scene.objects.push_back(*mesh);
        scene.passes = 1;
        renderer->Draw(scene);
    }
    else
    {
        isCube = true;
        Scene scene;
        scene.objects.push_back(*cube);
        scene.passes = 1;
        renderer->Draw(scene);
    }
}

void mouseRotate(MouseEvent *event, Program *prog)
{
    float degree = sqrt(pow(event->getPayload().relativeCoordinates[0], 2) + pow(event->getPayload().relativeCoordinates[1], 2));
    Vec3 axis = {event->getPayload().relativeCoordinates[1] / degree, event->getPayload().relativeCoordinates[0] / degree, 0};
    if(event->getPayload().state == true)
        rotateQuat(rot, degree, axis);
    prog->SetRotation(glm::mat4_cast(rot));
}

void bindEvents(CoreInterface* engine, UiLayer* ui, Program& program)
{
    Action<MouseEvent*> endGame(switchPerspective, std::placeholders::_1, &program);
    Action<MouseEvent*> rotate(mouseRotate, std::placeholders::_1, &program);
    Action<KeyboardEvent*> zoomObj(zoom, std::placeholders::_1, &program);
    Action<KeyboardEvent*> move(shiftKeyboard, std::placeholders::_1, &program);
    //Action<KeyboardEvent*> cube(cubeSwitch, std::placeholders::_1, frame, mesh, &renderer);
    engine->getEventHandler().setListener<KeyboardEvent>(move);
    engine->getEventHandler().setListener<KeyboardEvent>(zoomObj, [](EventInterface* e) { return e->getHint() == SDL_SCANCODE_EQUALS ||
                e->getHint() == SDL_SCANCODE_MINUS; });
    //engine->getEventHandler().setListener<KeyboardEvent>(cube, [](EventInterface* e) { return e->getHint() == SDL_SCANCODE_Q; });
    engine->getEventHandler().setListener<MouseEvent>(rotate, [](EventInterface* e) { return e->getHint() == integral(MouseData::Type::Motion); });
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
    program.SetOffset({0.0, 0.0});
    program.SetPerspective(pers);
    program.SetRotation(glm::mat4_cast(rotation));
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
        //mesh.Scale(4, -4);
        for (Material& mat : materials)
        {
            if (mat.name == mesh.getAttribute("material"))
            {
                for (Vertex& vertex : mesh.data().vertices)
                {
                    vertex.color = mat.diffuseColor;
                }
            }
        }
    }
    return meshes;
}

void drawMesh(std::vector<GraphicsObject>& mesh, Renderer& renderer, Program& prog)
{
    Scene scene;

    scene.passes = mesh.size();
    scene.objects = mesh;
    for (GraphicsObject& obj : scene.objects)
    {
        obj.data().program = &prog;
    }
    renderer.Draw(scene);
}
