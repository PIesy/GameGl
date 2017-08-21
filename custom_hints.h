//
// Created by akann on 6/7/17.
//

#ifndef GAMEGL_CUSTOM_HINTS_H
#define GAMEGL_CUSTOM_HINTS_H

#include "Framework/object_hints.h"

enum class CustomObjectHints : int {Skybox, FullScreenQuad};

ObjectHint asObjectHint(CustomObjectHints hint);

#endif //GAMEGL_CUSTOM_HINTS_H
