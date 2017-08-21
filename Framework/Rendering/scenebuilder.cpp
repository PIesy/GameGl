#include "scenebuilder.h"

Scene SceneBuilder::BuildScene(World& world, Camera& camera, const std::unordered_map<int, AdditionalStepParameters>& additionalParameters)
{
    Scene scene;
    std::vector<std::vector<ObjectHint>> hints;

    for (DrawableWorldObject& obj : world.GetDrawableObjects())
    {
        for (const Mesh& mesh : obj.GetMeshes())
        {
            Mat4 camMat = camera.GetCameraMatrix();
            Mat4 perspMat = camera.GetPerspectiveMatrix();
            Mat4 posMat = obj.GetPositionMatrix();
            Mat4 rotMat = obj.GetRotation();
            Vec3 camPosition = camera.GetPosition();
            Mat4 pMatrix = perspMat * camMat;
            MaterialProperties properties = mesh.GetMaterialProperties();
            int tiling = obj.GetTiling();
            bool aoTiling = obj.IsAoTiling();

            ConfigFunction c;
            c = [=](Program& p)
            {
                p.SetUniform(camPosition, "camPosition");
                p.SetUniform(camMat, "WtoCMatrix");
                p.SetUniform(perspMat, "perspective");
                p.SetUniform(posMat, "MtoWMatrix");
                p.SetUniform(rotMat, "rotation");
                p.SetUniform(pMatrix, "pMatrix");
                p.SetUniform(tiling, "tilingFactor");
                p.SetUniform(properties.roughnessMap, "roughnessTex");
                p.SetUniform(properties.aoMap, "aoTex");
                p.SetUniform(properties.albedoTexture, "albedoTex");
                p.SetUniform(properties.metallnessMap, "metallnessTex");
                p.SetUniform(properties.roughness, "roughness");
                p.SetUniform(properties.ao, "ao");
                p.SetUniform(properties.albedo, "albedo");
                p.SetUniform(properties.metallness, "metallness");
                p.SetUniform(properties.normalMap, "normalTex");
                p.SetUniform(properties.inverseRoughness, "inverseRoughness");
                p.SetUniform(aoTiling, "aoTiling");
            };

            scene.meshes.emplace_back(mesh, c);
            hints.emplace_back(obj.GetHints());
        }
    }

    int i = 0;

    for (auto& step : steps)
    {
        AdditionalStepParameters params;

        if (additionalParameters.count(i))
            params = additionalParameters.at(i);

        if (step.first.isMultiStep)
        {
            for (int j = 0; j < step.first.stepCount(world); j++)
            {
                step.first.processor(world, step.first.renderStep, i, j, params);
                scene.path.steps.push_back(step.first.renderStep);
                i++;
            }
        }
        else
        {
            step.first.processor(world, step.first.renderStep, i, 0, params);
            scene.path.steps.push_back(step.first.renderStep);
            i++;
        }
    }

    i = 0;
    for (MeshDescriptor& obj : scene.meshes)
    {
        int j = 0;
        for (auto& step : steps)
        {
            if (step.first.isMultiStep)
            {
                for (int k = 0; k < step.first.stepCount(world); k++, j++)
                {
                    if (step.second.empty() || contains(hints[i], step.second))
                        scene.path.steps[j].objects.push_back(i);
                }
            }
            else
            {
                if (step.second.empty() || contains(hints[i], step.second))
                    scene.path.steps[j].objects.push_back(i);
                j++;
            }
        }
        i++;
    }
    return scene;
}

void SceneBuilder::AddStep(const RenderStepWrapper& step, const std::vector<ObjectHint>& hints)
{
    steps.push_back({step, hints});
}

void SceneBuilder::AddStep(const RenderStepWrapper& step, ObjectHint hint)
{
    AddStep(step, std::vector<ObjectHint>{hint});
}

