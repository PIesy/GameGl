#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <unordered_map>

class Reader
{

};

class Loader
{
    std::unordered_map<std::string, Reader> readers;
public:
    Loader();
    void Register(const Reader& reader, const std::string& format);
};

#endif // LOADER_H
