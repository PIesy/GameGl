#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdint>
#include <string>

struct Texture
{
    void* data = nullptr;
    int64_t size = 0;
    unsigned width = 0;
    unsigned height = 0;
    unsigned componentsNum = 0;

    unsigned stepId = 0;
    unsigned textureId = 0;
    std::string name;
};

#endif // TEXTURE_H
