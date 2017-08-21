//
// Created by akann on 6/7/17.
//

#include "custom_hints.h"

ObjectHint asObjectHint(CustomObjectHints hint)
{
    return customHint(integral(hint));
}
