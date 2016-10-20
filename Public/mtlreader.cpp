#include "mtlreader.h"

void MtlReader::Analyze(std::string input)
{
    std::string type = input.substr(0, 2);
    Material mat;
    if (type == "ne")
    {
        std::string tmp = input.substr(7);
        mat.name = tmp;
        materials.push_back(mat);
    }
    if(type == "Kd")
        addDiffuseColor(input.substr(2));
}

void MtlReader::addDiffuseColor(std::string color)
{
    size_t cnt = 0;
    size_t offset = 0;
    float colors[3];

    for (int i = 0; i < 3; i++)
    {
        colors[i] = std::stof(color.substr(offset), &cnt);
        offset += cnt;
    }
    for (int j = 0; j < 3; j++)
    {
        materials.back().diffuseColor[j] = colors[j];
    }
}

std::vector<Material> MtlReader::getResult()
{
    return materials;
}

void MtlReader::Clear()
{
    vertexNumber = 0;
}
