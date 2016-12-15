#include "scenebuilder.h"

Scene SceneBuilder::BuildScene(World& world, Camera& camera)
{
    Scene scene;
    for (DrawableWorldObject& obj : world.getDrawableObjects())
    {
        GraphicsObject& mesh = obj.getObject();

        mesh.Configure("WtoCMatrix", camera.GetCameraMatrix());
        mesh.Configure("perspective", camera.GetPerspectiveMatrix());
        mesh.Configure("offset", Vec2(-1.0f, -1.0f));
        mesh.Configure("MtoWMatrix", obj.GetPositionMatrix());
        mesh.Configure("worldRotation", obj.getWorldRotation());
        mesh.Configure("rotation", obj.getRotation());
        scene.objects.push_back(mesh);
    }

    for (RenderStep& step : path.steps)
        scene.path.steps.emplace_back(step);

    int i = 0;
    for (GraphicsObject& obj : scene.objects)
    {
        for (RenderStep& step : scene.path.steps)
            step.objects.push_back(i);
        i++;
    }
    return scene;
}

void SceneBuilder::AddStep(const RenderStep& step)
{
    path.steps.push_back(step);
}
