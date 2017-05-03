#ifndef UNIFORM_H
#define UNIFORM_H

enum class UniformTypes: int {FLOAT = 0, INTEGER, UNSIGNED, DOUBLE};

struct UniformValue
{
    const void* value;
    UniformTypes type;
    unsigned count = 1;
    short horizontalSize = 1;
    short verticalSize = 1;
};

#endif // UNIFORM_H

