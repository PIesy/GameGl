//
// Created by akann on 5/2/17.
//

#ifndef GAMEGL_RENDERFLAGS_H
#define GAMEGL_RENDERFLAGS_H

#include <cstdint>
#include <vector>
#include "../Helpers/helpers.h"

enum class ObjectHint : int {CastsShadow = 1, LightEmitting, Custom};

constexpr ObjectHint customHint(int hint)
{
    return static_cast<ObjectHint>(integral(ObjectHint::Custom) + hint);
}

constexpr ObjectHint negative(ObjectHint hint)
{
    return static_cast<ObjectHint>(-integral(hint));
}

bool contains(const std::vector<ObjectHint>& hints, ObjectHint hint);
bool contains(const std::vector<ObjectHint>& src, const std::vector<ObjectHint>& ref);

#endif //GAMEGL_RENDERFLAGS_H
