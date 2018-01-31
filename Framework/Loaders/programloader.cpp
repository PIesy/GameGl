//
// Created by akann on 5/2/17.
//

#include "programloader.h"
#include "../../Public/shaderreader.h"
#include <fstream>
#include <sstream>

ProgramLoader::ProgramLoader(GraphicsApi& graphicsApi) : graphicsApi(graphicsApi) {}

std::vector<std::reference_wrapper<Shader>> ProgramLoader::Load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath)
{
    ShaderReader reader;
    std::string vertexSrc, geomSrc, fragSrc;
    std::vector<std::reference_wrapper<Shader>> shaders;
    std::stringstream buffer;

    std::ifstream file(vertexShaderPath);
    buffer << file.rdbuf();
    vertexSrc = buffer.str();
    shaders.push_back(std::ref(graphicsApi.CreateShader(vertexSrc, ShaderType::VertexShader)));

    buffer.str({});
    buffer.clear();
    if (!geometryShaderPath.empty())
    {
        file.close();
        file.clear();
        file.open(geometryShaderPath);
        buffer << file.rdbuf();
        geomSrc = buffer.str();
        shaders.push_back(std::ref(graphicsApi.CreateShader(geomSrc, ShaderType::GeometryShader)));
    }

    buffer.str({});
    buffer.clear();
    if (!fragmentShaderPath.empty())
    {
        file.close();
        file.clear();
        file.open(fragmentShaderPath);
        buffer << file.rdbuf();
        fragSrc = buffer.str();
        shaders.push_back(std::ref(graphicsApi.CreateShader(fragSrc, ShaderType::FragmentShader)));
    }

    return shaders;
}

