#include "scenebuilder.h"

Scene SceneBuilder::BuildScene(World& world, Camera& camera)
{
    Scene scene;
    for (DrawableWorldObject& obj : world.getDrawableObjects())
    {
        Mesh& mesh = obj.GetMesh();
        Mat4 camMat = camera.GetCameraMatrix();
        Mat4 perspMat = camera.GetPerspectiveMatrix();
        Mat4 posMat = obj.GetPositionMatrix();
        Mat4 worldRotMat = obj.GetWorldRotation();
        Mat4 rotMat = obj.GetRotation();

        ConfigFunction c = [=](Program& p)
        {
            p.setUniform(camMat, "WtoCMatrix");
            p.setUniform(perspMat, "perspective");
            p.setUniform(Vec2{-1.0f, -1.0f}, "offset");
            p.setUniform(posMat, "MtoWMatrix");
            p.setUniform(worldRotMat, "worldRotation");
            p.setUniform(rotMat, "rotation");
        };
        scene.meshes.emplace_back(mesh, c);
    }

    for (RenderStep& step : path.steps)
        scene.path.steps.emplace_back(step);

    int i = 0;
    for (MeshDescriptor& obj : scene.meshes)
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
