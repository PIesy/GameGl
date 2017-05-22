#include "scenebuilder.h"

Scene SceneBuilder::BuildScene(World& world, Camera& camera)
{
    Scene scene;
    std::vector<RenderFlags> flags;

    for (DrawableWorldObject& obj : world.GetDrawableObjects())
    {
        for (const Mesh& mesh : obj.GetMeshes())
        {
            Mat4 camMat = camera.GetCameraMatrix();
            Mat4 perspMat = camera.GetPerspectiveMatrix();
            Mat4 posMat = obj.GetPositionMatrix();
            Mat4 worldRotMat = obj.GetWorldRotation();
            Mat4 rotMat = obj.GetRotation();
            Vec3 camPosition = camera.GetPosition();

            ConfigFunction c;
            c = [=](Program& p)
            {
                p.SetUniform(camPosition, "camPosition");
                p.SetUniform(camMat, "WtoCMatrix");
                p.SetUniform(perspMat, "perspective");
                p.SetUniform(posMat, "MtoWMatrix");
                p.SetUniform(rotMat, "rotation");
            };

            scene.meshes.emplace_back(mesh, c);
            flags.push_back(obj.GetRenderFlags());
        }
    }

    int i = 0;

    std::vector<Vec3> lightPositions;
    std::vector<Vec3> lightColors;
    std::vector<float> lightFarPlanes;
    for (Light& light : world.GetLights())
    {
        lightPositions.push_back(light.GetPosition());
        lightColors.push_back(light.GetLightColor());
        lightFarPlanes.push_back(light.GetMaxDistance());
    }

    for (auto& step : steps)
    {
        scene.path.steps.emplace_back(step.first);
        scene.path.steps.back().preConfig = [=](Program& p)
        {
            p.SetUniform(lightPositions[0], "lightPositions", (int)lightPositions.size());
            p.SetUniform(lightColors[0], "lightColors", (int)lightColors.size());
            p.SetUniform((int)lightPositions.size(), "actualLightsCount");
        };
    }

    i = 0;
    for (MeshDescriptor& obj : scene.meshes)
    {
        int j = 0;
        for (RenderStep& step : scene.path.steps)
        {
            if (steps[j].second == RenderFlags::Default || contains(flags[i], steps[j].second))
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
