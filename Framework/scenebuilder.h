#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include "world.h"
#include "Graphics/scene.h"

class SceneBuilder
{
    RenderPath path;
public:
    Scene BuildScene(World& world, Camera& camera);
    void AddStep(const RenderStep& step);
};

#endif // SCENEBUILDER_H
