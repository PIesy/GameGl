#ifndef UNIFORM_H
#define UNIFORM_H

#include <memory>

enum class UniformTypes: int {FLOAT = 0, INTEGER, UNSIGNED, DOUBLE};

struct UniformValue
{
    void* value;
    UniformTypes type;
    unsigned count = 1;
    short horizontalSize = 1;
    short verticalSize = 1;
};

#endif // UNIFORM_H

