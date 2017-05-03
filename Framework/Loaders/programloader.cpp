//
// Created by akann on 5/2/17.
//

#include "programloader.h"
#include "../../Public/shaderreader.h"

ProgramLoader::ProgramLoader(ResourcesApi& resourcesApi, GraphicsApi& graphicsApi) : resourcesApi(resourcesApi), graphicsApi(graphicsApi) {}

Program& ProgramLoader::Load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath)
{
    ShaderReader reader;
    std::string vertexSrc, geomSrc, fragSrc;
    std::vector<std::reference_wrapper<Shader>> shaders;

    resourcesApi.ReadByTemplate(vertexShaderPath, reader);
    vertexSrc = reader.getResult();
    shaders.push_back(std::ref(graphicsApi.CreateShader(vertexSrc, ShaderType::VertexShader)));

    if (!geometryShaderPath.empty())
    {
        resourcesApi.ReadByTemplate(geometryShaderPath, reader);
        geomSrc = reader.getResult();
        shaders.push_back(std::ref(graphicsApi.CreateShader(geomSrc, ShaderType::GeometryShader)));
    }

    if (!fragmentShaderPath.empty())
    {
        resourcesApi.ReadByTemplate(fragmentShaderPath, reader);
        fragSrc = reader.getResult();
        shaders.push_back(std::ref(graphicsApi.CreateShader(fragSrc, ShaderType::FragmentShader)));
    }

    Program& program = graphicsApi.CreateProgram();

    for (Shader& shader : shaders)
        program.Attach(shader);
    program.Compile();
    return program;
}

