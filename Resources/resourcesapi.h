#ifndef RESOURCESAPI_H
#define RESOURCESAPI_H

#include <string>
#include "Api/apibase.h"

class Scanner
{
public:
    virtual ~Scanner(){}
    virtual void Analyze(std::string) {}
    virtual void* Fill(){ return nullptr; }
};

class ResourcesApi: public ApiBase
{
public:
    virtual ~ResourcesApi() {}
    virtual void* ReadByTemplate(std::string source, Scanner* scanner) = 0;
};

#endif // RESOURCESAPI_H
