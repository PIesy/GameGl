//
// Created by akann on 6/9/17.
//

#ifndef GAMEGL_STEP_COLLECTION_H
#define GAMEGL_STEP_COLLECTION_H

#include <unordered_map>
#include "Framework/Rendering/RenderStepWrapper.h"

enum class StepNames {PointShadows, GlobalShadows, HdrMapBuilder, BrdfBuilder, PbrRenderer,
    SkyboxRenderer, TextureViewer, DiffuseMapBuilder, SpecularMapBuilder, Generic};

struct Context
{
    std::vector<Texture> textures;
    Vec2 resolution;
};

class StepBuilder
{

    RenderStepWrapper buildPointShadowsStep(World& world, const std::vector<RenderStepWrapper>& dependencies, std::vector<std::reference_wrapper<Shader>> program);
    RenderStepWrapper buildGlobalShadowsStep(World& world, const std::vector<RenderStepWrapper>& dependencies, std::vector<std::reference_wrapper<Shader>> program);
    RenderStepWrapper buildHdrMapBuilderStep(World& world, const std::vector<RenderStepWrapper>& dependencies, const Context& context, std::vector<std::reference_wrapper<Shader>> program);
    RenderStepWrapper buildBrdfBuilderStep(World& world, const std::vector<RenderStepWrapper>& dependencies);
    RenderStepWrapper buildPbrRendererStep(World& world, const std::vector<RenderStepWrapper>& dependencies, const Context& context, std::vector<std::reference_wrapper<Shader>> program);
    RenderStepWrapper buildSkyboxRendererStep(World& world, const std::vector<RenderStepWrapper>& dependencies, const Context& context);
    RenderStepWrapper buildTextureViewerStep(World& world, const std::vector<RenderStepWrapper>& dependencies, const Context& context);
    RenderStepWrapper buildDiffuseMapBuilderStep(World& world, const std::vector<RenderStepWrapper>& dependencies, std::vector<std::reference_wrapper<Shader>> program);
    RenderStepWrapper buildSpecularMapBuilderStep(World& world, const std::vector<RenderStepWrapper>& dependencies, std::vector<std::reference_wrapper<Shader>> program);
    RenderStepWrapper buildGenericStep(World& world, const std::vector<RenderStepWrapper>& dependencies);
public:
    RenderStepWrapper Build(StepNames step, World& world, std::vector<std::reference_wrapper<Shader>> program, const std::vector<RenderStepWrapper>& dependencies, const Context& context = {});
};


#endif //GAMEGL_STEP_COLLECTION_H
