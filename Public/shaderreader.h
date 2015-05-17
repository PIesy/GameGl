#ifndef SHADERREADER_H
#define SHADERREADER_H

#include "Resources/resourcesapi.h"

class ShaderReader: public Scanner
{
    std::string result;
public:
    void Analyze(std::string input);
    std::string getResult();
    void Clear();
};

#endif // SHADERREADER_H
