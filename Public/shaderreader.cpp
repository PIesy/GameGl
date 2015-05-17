#include "shaderreader.h"

void ShaderReader::Analyze(std::string input)
{
    result.append(input);
    result.append("\n");
}

std::string ShaderReader::getResult()
{
    return result;
}

void ShaderReader::Clear()
{
    result.clear();
}
