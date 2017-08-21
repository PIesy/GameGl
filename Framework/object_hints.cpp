//
// Created by akann on 6/7/17.
//

#include <vector>
#include "object_hints.h"

bool contains(const std::vector<ObjectHint>& hints, ObjectHint hint)
{
    ObjectHint val = hint;
    bool result = true;

    if (integral(hint) < 0)
    {
        result = false;
        val = static_cast<ObjectHint>(-integral(hint));
    }

    for (const ObjectHint& h : hints)
    {
        if (h == val)
            return result;
    }
    return !result;
}

bool contains(const std::vector<ObjectHint>& src, const std::vector<ObjectHint>& ref)
{
    for (const ObjectHint& hint : ref)
    {
        if (!contains(src, hint))
            return false;
    }
    return true;
}