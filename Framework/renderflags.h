//
// Created by akann on 5/2/17.
//

#ifndef GAMEGL_RENDERFLAGS_H
#define GAMEGL_RENDERFLAGS_H

#include <cstdint>
#include "../Helpers/helpers.h"

enum class RenderFlags : uint64_t
{
    Default = 0,
    CastsShadow = 1,
    SkyBox = 1 << 1,
    LightEmitting = 1 << 2,
    FullScreenQuad = 1 << 3
};

constexpr RenderFlags operator|(const RenderFlags& left, const RenderFlags& right)
{
    return static_cast<RenderFlags>(integral(left) | integral(right));
}

constexpr bool contains(const RenderFlags& left, const RenderFlags& right)
{
    return static_cast<bool>(integral(left) & integral(right));
}


#endif //GAMEGL_RENDERFLAGS_H
