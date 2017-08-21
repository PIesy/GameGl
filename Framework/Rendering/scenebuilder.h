#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include "../world.h"
#include "RenderStepWrapper.h"
#include "../../Graphics/scene.h"
#include <unordered_map>

class SceneBuilder
{
    std::vector<std::pair<RenderStepWrapper, std::vector<ObjectHint>>> steps;
public:
    Scene BuildScene(World& world, Camera& camera, const std::unordered_map<int, AdditionalStepParameters>& additionalParameters = {});
    void AddStep(const RenderStepWrapper& step, const std::vector<ObjectHint>& hints = {});
    void AddStep(const RenderStepWrapper& step, ObjectHint hint);
};

#endif // SCENEBUILDER_H
