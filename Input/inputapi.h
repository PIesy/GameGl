#ifndef INPUTAPI_H
#define INPUTAPI_H

#include "../Core/apibase.h"
#include "inputdata.h"

class InputApi: public ApiBase
{
public:
    virtual ~InputApi() {}
    virtual KeyboardData GetKeyboardState() = 0;
    virtual MouseData GetMouseState() = 0;
};

#endif // INPUTAPI_H
