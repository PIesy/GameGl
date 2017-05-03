#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include "world.h"
#include "../Graphics/scene.h"

class SceneBuilder
{
    std::vector<std::pair<RenderStep, RenderFlags>> steps;
public:
    Scene BuildScene(World& world, Camera& camera);
    void AddStep(const RenderStep& step, RenderFlags flags = RenderFlags::Default);
};

#endif // SCENEBUILDER_H
