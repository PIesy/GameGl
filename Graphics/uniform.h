#ifndef UNIFORM_H
#define UNIFORM_H

class Uniform
{
public:
    virtual void Use();
};

template<class T>
class TypedUniform: public Uniform
{
protected:
    T value;
public:
    TypedUniform(const T& value);
    virtual void Use();
};

#endif // UNIFORM_H

