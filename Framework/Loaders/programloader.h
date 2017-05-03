//
// Created by akann on 5/2/17.
//

#ifndef GAMEGL_PROGRAMLOADER_H
#define GAMEGL_PROGRAMLOADER_H


#include "../../Graphics/graphicsclasses.h"
#include "../../Public/apis.h"

class ProgramLoader
{
    ResourcesApi& resourcesApi;
    GraphicsApi& graphicsApi;
public:
    ProgramLoader(ResourcesApi& resourcesApi, GraphicsApi& graphicsApi);
    Program& Load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath = "", const std::string& geometryShaderPath = "");
};


#endif //GAMEGL_PROGRAMLOADER_H
