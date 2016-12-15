#include "loader.h"

Loader::Loader()
{

}

void Loader::Register(const Reader& reader, const std::string& format)
{
    if (!readers.count(format))
        readers.insert({format, reader});
}
