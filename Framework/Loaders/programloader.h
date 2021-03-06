//
// Created by akann on 5/2/17.
//

#ifndef GAMEGL_PROGRAMLOADER_H
#define GAMEGL_PROGRAMLOADER_H


#include "../../Graphics/graphicsclasses.h"
#include "../../Public/apis.h"

class ProgramLoader
{
    GraphicsApi& graphicsApi;
public:
    explicit ProgramLoader(GraphicsApi& graphicsApi);
    std::vector<std::reference_wrapper<Shader>> Load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath = "", const std::string& geometryShaderPath = "");
};


#endif //GAMEGL_PROGRAMLOADER_H
