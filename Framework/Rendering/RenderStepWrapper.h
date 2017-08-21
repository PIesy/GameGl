//
// Created by akann on 6/9/17.
//

#ifndef GAMEGL_RENDERSTEPWRAPPER_H
#define GAMEGL_RENDERSTEPWRAPPER_H

#include <functional>
#include "../world.h"
#include "../../Graphics/renderstep.h"

struct AdditionalStepParameters
{
    RenderingAttributes customAttributes;
    std::function<void(Program&)> additionalShaderConfiguration = [](Program&){};
};

struct RenderStepWrapper
{
    bool isMultiStep = false;
    std::function<void(World&, RenderStep&, unsigned baseStepId, int invocationId, AdditionalStepParameters&)> processor =
            [](World&, RenderStep& step, unsigned, int, AdditionalStepParameters& parameters)
            {
            };
    std::function<int(World&)> stepCount;
    RenderStep renderStep;
};

#endif //GAMEGL_RENDERSTEPWRAPPER_H
