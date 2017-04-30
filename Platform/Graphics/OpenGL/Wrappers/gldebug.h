#ifndef GLDEBUG_H
#define GLDEBUG_H

#include <string>
#include "glbindings.h"

namespace gl
{

    void printGlError(const std::string& message);
    void registerDebugCallback();

}


#endif // GLDEBUG_H

