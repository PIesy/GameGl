//
// Created by akann on 6/9/17.
//

#include "step_builder.h"
#include "Framework/texturebuilder.h"

RenderStepWrapper StepBuilder::buildPointShadowsStep(World& world, const std::vector<RenderStepWrapper>& dependencies)
{
    RenderStepWrapper result;
    RenderStep step;
    Texture texture = TextureBuilder::BuildCubemap(TextureBindpoint::Depth, 1024, 1024,
                                                   world.GetPointLights().size() + 1, 1, TexturePixelFormat::Float24, false);

    //texture.parameters.sampling = TextureSampling::Nearest;
    step.targets.push_back(RenderTarget(texture));
    step.attributes = {RenderingAttribute::DepthTest, RenderingAttribute::ClearBuffer};
    result.renderStep = step;
    result.isMultiStep = true;
    result.stepCount = [](World& w)
    {
        return w.GetPointLights().size();
    };
    result.processor = [](World& world, RenderStep& step, unsigned baseStepId, int invocationId, AdditionalStepParameters& parameters)
    {
        if (step.targets.front().texture.info.count != ((world.GetPointLights().size() + 1) * 6))
            step.targets = {RenderTarget(TextureBuilder::BuildCubemap(TextureBindpoint::Depth, 1024, 1024,
                                                                      world.GetPointLights().size() + 1, 1, TexturePixelFormat::Float24, false))};
        Light& light = world.GetPointLights()[invocationId];
        if (invocationId != 0)
        {
            step.attributes = {RenderingAttribute::DepthTest};
            step.frameBufferProperties.frameBufferType = FrameBufferProperties::REUSE_FROM_STEP;
            step.frameBufferProperties.stepId = baseStepId;
        }
        step.preConfig = [light, invocationId](Program& p)
        {
            p.SetUniform(invocationId, "baseLayer");
            p.SetUniform(light.GetCubeMapMatrices()[0], "projections", 6);
            p.SetUniform(light.GetPerspectiveMatrix(), "tMatrix");
            p.SetUniform(light.GetMaxDistance(), "farPlane");
            p.SetUniform(light.GetPosition(), "lightPosition");
        };
    };
    return result;
}

RenderStepWrapper StepBuilder::buildGlobalShadowsStep(World& world, const std::vector<RenderStepWrapper>& dependencies)
{
    RenderStepWrapper result;
    RenderStep step;
    Texture globalShadow = TextureBuilder::Build2DTexture(TextureBindpoint::Depth, 8192, 8192,
                                                          world.GetDirectionalLights().size() + 1, 1, TexturePixelFormat::Float24, false);

    //globalShadow.parameters.sampling = TextureSampling::Nearest;
    step.attributes = {RenderingAttribute::DepthTest, RenderingAttribute::ClearBuffer};
    step.targets.push_back(RenderTarget(globalShadow));
    result.renderStep = step;
    result.isMultiStep = true;
    result.stepCount = [](World& w)
    {
        return w.GetDirectionalLights().size();
    };
    result.processor = [](World& world, RenderStep& step, unsigned baseStepId, int invocationId, AdditionalStepParameters& parameters)
    {
        if (step.targets.front().texture.info.count != (world.GetDirectionalLights().size() + 1))
            step.targets = {RenderTarget(TextureBuilder::Build2DTexture(TextureBindpoint::Depth, 4096, 4096,
                                                                        world.GetDirectionalLights().size() + 1, 1, TexturePixelFormat::Float24, false))};
        Light& light = world.GetDirectionalLights()[invocationId];
        if (invocationId != 0)
        {
            step.attributes = {RenderingAttribute::DepthTest};
            step.frameBufferProperties.frameBufferType = FrameBufferProperties::REUSE_FROM_STEP;
            step.frameBufferProperties.stepId = baseStepId;
        }
        Mat4 matrix = light.GetPerspectiveMatrix() * light.GetTranslationMatrix();

        step.preConfig = [matrix, invocationId](Program& p)
        {
            p.SetUniform(invocationId, "baseLayer");
            p.SetUniform(matrix, "tMatrix");
        };
    };
    return result;
}

RenderStepWrapper StepBuilder::buildHdrMapBuilderStep(World& world, const std::vector<RenderStepWrapper>& dependencies, const Context& context)
{
    RenderStepWrapper result;
    RenderStep step;
    Light fakeLight{LightType::Point};

    Texture hdrSkybox = TextureBuilder::BuildCubemap(TextureBindpoint::Color, 1024, 1024, 1, 4, TexturePixelFormat::Float16, true);
    step.genericTextures = {context.textures.front()};
    step.targets.push_back(RenderTarget(hdrSkybox));
    step.attributes = {RenderingAttribute::UpdateMipmaps, RenderingAttribute::ClearBuffer, RenderingAttribute::DepthTest, RenderingAttribute::Blend};
    step.postConfig = [=](Program& p)
    {
        p.SetUniform(fakeLight.GetCubeMapMatrices()[0], "projections", 6);
        p.SetUniform(fakeLight.GetPerspectiveMatrix(), "perspective");
    };
    result.renderStep = step;

    return result;
}

RenderStepWrapper StepBuilder::buildBrdfBuilderStep(World& world, const std::vector<RenderStepWrapper>& dependencies)
{
    RenderStepWrapper result;
    RenderStep step;
    Texture texture = TextureBuilder::Build2DTexture(TextureBindpoint::Color, 512, 512, 1, 3, TexturePixelFormat::Float16, false);

    step.attributes = {RenderingAttribute::ClearBuffer};
    step.targets.push_back(RenderTarget(texture));
    result.renderStep = step;
    return result;
}

RenderStepWrapper StepBuilder::buildPbrRendererStep(World& world, const std::vector<RenderStepWrapper>& dependencies, const Context& context)
{
    RenderStepWrapper result;
    RenderStep step;
    Texture hdrTexture = TextureBuilder::Build2DTexture(TextureBindpoint::Color, context.resolution.x, context.resolution.y, 1, 4, TexturePixelFormat::Float32, false);

    for (const RenderStepWrapper& wrapper : dependencies)
        step.genericTextures.push_back(wrapper.renderStep.targets.front().texture);
    step.targets.push_back(RenderTarget(hdrTexture));
    result.renderStep = step;
    result.processor = [](World& world, RenderStep& step, unsigned baseStepId, int invocationId, AdditionalStepParameters& parameters)
    {
        if (!parameters.customAttributes.empty())
            step.attributes = parameters.customAttributes;
        std::vector<Vec3> lightPositions;
        std::vector<Vec3> lightColors;
        std::vector<float> lightFarPlanes;
        std::vector<Vec3> directionalLights;
        std::vector<Vec3> directionalLightsColor;
        std::vector<Mat4> directionalTranslationMatrices;
        for (Light& light : world.GetPointLights())
        {
            lightPositions.push_back(light.GetPosition());
            lightColors.push_back(light.GetLightColor());
            lightFarPlanes.push_back(light.GetMaxDistance());
        }
        for (Light light : world.GetDirectionalLights())
        {
            directionalLights.push_back(light.GetDirection());
            directionalLightsColor.push_back(light.GetLightColor());
            directionalTranslationMatrices.push_back(light.GetPerspectiveMatrix() * light.GetTranslationMatrix());
        }

        step.preConfig = [=](Program& p)
        {
            p.SetUniform(directionalLights[0], "directionalLights", (int) directionalLights.size());
            p.SetUniform(directionalLightsColor[0], "directionalLightsColor", (int) directionalLightsColor.size());
            p.SetUniform(directionalTranslationMatrices[0], "directionalLightsMatrices", (int) directionalTranslationMatrices.size());
            p.SetUniform(lightPositions[0], "lightPositions", (int) lightPositions.size());
            p.SetUniform(lightColors[0], "lightColors", (int) lightColors.size());
            p.SetUniform(lightFarPlanes[0], "lightFarPlanes", (int) lightColors.size());
            p.SetUniform((int) lightPositions.size(), "actualLightsCount");
            p.SetUniform((int) directionalLights.size(), "directionalLightsCount");
        };
        step.postConfig = parameters.additionalShaderConfiguration;
    };

    return result;
}

RenderStepWrapper StepBuilder::buildSkyboxRendererStep(World& world, const std::vector<RenderStepWrapper>& dependencies, const Context& context)
{
    RenderStepWrapper result;
    RenderStep step;

    step.frameBufferProperties.frameBufferType = FrameBufferProperties::REUSE_FROM_STEP;
    step.attributes = {RenderingAttribute::DepthTest};
    for (const RenderStepWrapper& wrapper : dependencies)
        step.genericTextures.push_back(wrapper.renderStep.targets.front().texture);
    if (!context.textures.empty())
        step.genericTextures.push_back(context.textures[0]);
    result.renderStep = step;
    result.processor = [](World&, RenderStep& step, unsigned baseStepId, int, AdditionalStepParameters&)
    {
        step.frameBufferProperties.stepId = baseStepId - 1;
    };

    return result;
}

RenderStepWrapper StepBuilder::buildTextureViewerStep(World& world, const std::vector<RenderStepWrapper>& dependencies, const Context& context)
{
    RenderStepWrapper result;
    RenderStep step;

    for (const RenderStepWrapper& wrapper : dependencies)
        step.genericTextures.push_back(wrapper.renderStep.targets.front().texture);
    for (const Texture& tex : context.textures)
        step.genericTextures.push_back(tex);
    result.renderStep = step;
    return result;
}

RenderStepWrapper StepBuilder::buildDiffuseMapBuilderStep(World& world, const std::vector<RenderStepWrapper>& dependencies)
{
    RenderStepWrapper result;
    RenderStep step;
    Light fakeLight{LightType::Point};

    Texture hdrSkyboxConv = TextureBuilder::BuildCubemap(TextureBindpoint::Color, 32, 32, 1, 4, TexturePixelFormat::Float16, false);
    step.targets.push_back(RenderTarget(hdrSkyboxConv));
    step.postConfig = [=](Program& p)
    {
        p.SetUniform(fakeLight.GetCubeMapMatrices()[0], "projections", 6);
        p.SetUniform(fakeLight.GetPerspectiveMatrix(), "perspective");
    };
    for (const RenderStepWrapper& wrapper : dependencies)
        step.genericTextures.push_back(wrapper.renderStep.targets.front().texture);
    result.renderStep = step;
    return result;
}

RenderStepWrapper StepBuilder::buildSpecularMapBuilderStep(World& world, const std::vector<RenderStepWrapper>& dependencies)
{
    RenderStepWrapper result;
    RenderStep step;
    Light fakeLight{LightType::Point};
    const int mipmaps = 5;

    Texture hdrSkyboxPref = TextureBuilder::BuildCubemap(TextureBindpoint::Color, 128, 128, 1, 4, TexturePixelFormat::Float16, false);
    hdrSkyboxPref.info.mipmaps = mipmaps - 1;
    step.targets = {RenderTarget{hdrSkyboxPref}};
    for (const RenderStepWrapper& wrapper : dependencies)
        step.genericTextures.push_back(wrapper.renderStep.targets.front().texture);
    result.renderStep = step;
    result.isMultiStep = true;
    result.stepCount = [mipmaps](World&)
    {
        return mipmaps;
    };
    result.processor = [hdrSkyboxPref, fakeLight](World& world, RenderStep& step, unsigned baseStepId, int invocationId, AdditionalStepParameters& parameters)
    {
        Texture t = hdrSkyboxPref;
        t.parameters.useMipLevel = invocationId;
        step.targets = {RenderTarget(t)};
        step.postConfig = [=](Program& p)
        {
            p.SetUniform(fakeLight.GetCubeMapMatrices()[0], "projections", 6);
            p.SetUniform(fakeLight.GetPerspectiveMatrix(), "perspective");
            p.SetUniform((float) invocationId / mipmaps, "roughness");
        };
    };
    return result;
}


RenderStepWrapper StepBuilder::buildGenericStep(World& world, const std::vector<RenderStepWrapper>& dependencies)
{
    RenderStepWrapper result;
    RenderStep step;

    step.frameBufferProperties.frameBufferType = FrameBufferProperties::REUSE_FROM_STEP;
    step.attributes = {RenderingAttribute::WireframeMode};
    result.renderStep = step;
    result.processor = [](World& world, RenderStep& step, unsigned baseStepId, int invocationId, AdditionalStepParameters& parameters)
    {
        step.frameBufferProperties.stepId = baseStepId - 1;
    };

    return result;
}

RenderStepWrapper StepBuilder::Build(StepNames step, World& world, Program* program, const std::vector<RenderStepWrapper>& dependencies, const Context& context)
{
    RenderStepWrapper result;

    switch (step)
    {
        case StepNames::PointShadows:
            result = buildPointShadowsStep(world, dependencies);
            break;
        case StepNames::GlobalShadows:
            result = buildGlobalShadowsStep(world, dependencies);
            break;
        case StepNames::HdrMapBuilder:
            result = buildHdrMapBuilderStep(world, dependencies, context);
            break;
        case StepNames::BrdfBuilder:
            result = buildBrdfBuilderStep(world, dependencies);
            break;
        case StepNames::PbrRenderer:
            result = buildPbrRendererStep(world, dependencies, context);
            break;
        case StepNames::SkyboxRenderer:
            result = buildSkyboxRendererStep(world, dependencies, context);
            break;
        case StepNames::TextureViewer:
            result = buildTextureViewerStep(world, dependencies, context);
            break;
        case StepNames::DiffuseMapBuilder:
            result = buildDiffuseMapBuilderStep(world, dependencies);
            break;
        case StepNames::SpecularMapBuilder:
            result = buildSpecularMapBuilderStep(world, dependencies);
            break;
        case StepNames::Generic:
            result = buildGenericStep(world, dependencies);
            break;
    }
    result.renderStep.prog = program;
    return result;
}
