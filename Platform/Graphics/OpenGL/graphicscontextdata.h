//
// Created by akann on 4/14/17.
//

#ifndef GAMEGL_GRAPHICSCONTEXTDATA_H
#define GAMEGL_GRAPHICSCONTEXTDATA_H

#include <string>
#include <unordered_map>
#include "Wrappers/gltexture.h"
#include "Wrappers/glbuffer.h"
#include "../../../Helpers/shared_thread_local_wrapper.h"


struct GraphicsContextData
{
    std::unordered_map<std::string, gl::GlTexture> textureMap;
    std::unordered_multimap<std::string, gl::GlBuffer> bufferMap;
};

extern thread_local SharedThreadLocalWrapper<GraphicsContextData> globalData;

#endif //GAMEGL_GRAPHICSCONTEXTDATA_H
