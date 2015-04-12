#ifndef IOMODULE
#define IOMODULE

#include "module.h"
#include "IO/ioapi.h"

class IoModule: public Module
{
    ModuleApi<IoApi>* api = nullptr;
public:
    const void* getApi();
    ~IoModule();
    void setApi(void* api);
};

#endif // IOMODULE

