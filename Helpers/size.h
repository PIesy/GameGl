#ifndef SIZE_H
#define SIZE_H

struct Size
{
    int width;
    int height;
};

struct Size3d: public Size
{
    int depth;
};

#endif // SIZE_H

