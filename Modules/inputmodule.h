#ifndef IOMODULE
#define IOMODULE

#include "module.h"
#include "Input/inputapi.h"

class InputModule: public Module
{
    ModuleApi<InputApi>* api = nullptr;
public:
    const void* getApi();
    ~InputModule();
    void setApi(void* api);
};

#endif // IOMODULE

