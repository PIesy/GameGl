#ifndef IOMODULE
#define IOMODULE

#include "module.h"
#include "../Input/inputapi.h"

class InputModule: public Module
{
    ModuleApi<InputApi>* api = nullptr;
public:
    InputApi* getApi();
    ~InputModule();
    void setApi(ApiBase* api);
};

#endif // IOMODULE

