//
// Created by akann on 4/14/17.
//

#ifndef GAMEGL_GRAPHICSCONTEXTDATA_H
#define GAMEGL_GRAPHICSCONTEXTDATA_H

#include <string>
#include <unordered_map>
#include <shared_mutex>
#include "Wrappers/gltexture.h"
#include "Wrappers/glbuffer.h"
#include "../../../Helpers/shared_thread_local_wrapper.h"
#include "../../../Helpers/synchronized_map.h"
#include "Wrappers/glprogram.h"

struct SharedGlContextData
{
    SynchronizedMap<std::string, gl::GlTexture> textureMap;
    SynchronizedMap<std::string, gl::GlBuffer> bufferMap;
    SynchronizedMap<size_t, gl::GlProgram> programMap;
};

extern thread_local SharedThreadLocal<SharedGlContextData> sharedGlData;

#endif //GAMEGL_GRAPHICSCONTEXTDATA_H
