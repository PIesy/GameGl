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
    Texture Load(const std::string& path, bool useHDR = false);
    Texture LoadCubemap(const std::array<std::string, 6>& files, bool useHDR = false);
    Texture LoadArray(const std::vector<std::string>& files, bool useHDR = false);
};


#endif //GAMEGL_TEXTURELOADER_H
