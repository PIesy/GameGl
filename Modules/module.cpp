#include "module.h"

void Module::setEngine(EngineInterface& engine)
{
    this->engine = &engine;
}
