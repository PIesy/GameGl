#ifndef IOAPI
#define IOAPI

#include "Core/invokable.h"
#include "devid.h"
#include "Api/apibase.h"

class IoApi: public ApiBase
{
public:
    virtual ~IoApi() {}
    virtual void SendData(void* data, DeviceIdentifier id) = 0;
    virtual void* GetData(DeviceIdentifier id) = 0;
};

#endif // IOAPI
