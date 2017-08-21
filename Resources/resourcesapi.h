#ifndef RESOURCESAPI_H
#define RESOURCESAPI_H

#include <string>
#include "../Core/apibase.h"

class Scanner
{
public:
    virtual ~Scanner() {}
    virtual void Analyze(std::string) = 0;
    virtual void Clear() = 0;
};

class ResourcesApi: public ApiBase
{
public:
    virtual ~ResourcesApi() {}
    virtual bool ReadByTemplate(std::string source, Scanner& scanner) = 0;
};

#endif // RESOURCESAPI_H
