#ifndef MTLREADER_H
#define MTLREADER_H

#include <vector>
#include "../Resources/resourcesapi.h"
#include "../Graphics/graphicsclasses.h"

struct Material
{
    Vec4 diffuseColor = {1, 1, 1, 1};
    std::string name = "N/A";
};

class MtlReader : public Scanner
{
    std::vector<Material> materials;
    void addDiffuseColor(std::string color);
    int vertexNumber = 0;
public:
    MtlReader() {}
    void Analyze(std::string input);
    void Clear();
    std::vector<Material> getResult();
};

#endif // MTLREADER_H
