#include "scenebuilder.h"

Scene SceneBuilder::BuildScene(World& world, Camera& camera)
{
    Scene scene;
    std::vector<RenderFlags> flags;

    for (DrawableWorldObject& obj : world.getDrawableObjects())
    {
        Mesh& mesh = obj.GetMesh();
        Mat4 camMat = camera.GetCameraMatrix();
        Mat4 perspMat = camera.GetPerspectiveMatrix();
        Mat4 posMat = obj.GetPositionMatrix();
        Mat4 worldRotMat = obj.GetWorldRotation();
        Mat4 rotMat = obj.GetRotation();
        Vec3 camPosition = camera.GetPosition();
        float useTex = mesh.GetTextures().size() > 0 ? 1.0f : 0.0f;

        ConfigFunction c;
        c = [=](Program& p)
        {
            p.SetUniform(useTex, "useTex");
            p.SetUniform(0.0f, "debug");
            p.SetUniform(camPosition, "camPosition");
            p.SetUniform(camMat, "WtoCMatrix");
            p.SetUniform(perspMat, "perspective");
            p.SetUniform(posMat, "MtoWMatrix");
            p.SetUniform(worldRotMat, "worldRotation");
            p.SetUniform(rotMat, "rotation");
        };

        scene.meshes.emplace_back(mesh, c);
        flags.push_back(obj.GetRenderFlags());
    }

    for (auto& step : steps)
        scene.path.steps.emplace_back(step.first);

    int i = 0;
    for (MeshDescriptor& obj : scene.meshes)
    {
        int j = 0;
        for (RenderStep& step : scene.path.steps)
        {
            if (contains(flags[i], steps[j].second) || steps[j].second == RenderFlags::Default)
                step.objects.push_back(i);
            j++;
        }
        i++;
    }
    return scene;
}

void SceneBuilder::AddStep(const RenderStep& step, RenderFlags flags)
{
    steps.push_back({step, flags});
}
