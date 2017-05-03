//
// Created by akann on 4/25/17.
//

#ifndef GAMEGL_TEXTURELOADER_H
#define GAMEGL_TEXTURELOADER_H

#include "../../Public/coreinterface.h"

class TextureLoader
{
    StorageApi& api;
public:
    TextureLoader(StorageApi& storage);
    Texture Load(const std::string& path);
    Texture LoadCubemap(const std::vector<std::string>& files);
};


#endif //GAMEGL_TEXTURELOADER_H
