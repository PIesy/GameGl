#ifndef FILEREADER_H
#define FILEREADER_H

#include "resourcesapi.h"
#include <fstream>

class FileReader: public ResourcesApi
{
public:
    FileReader(){}
    bool ReadByTemplate(std::string source, Scanner& scanner);
};

#endif // FILEREADER_H
