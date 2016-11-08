#include "graphicsclasses.h"


template<>
InvokationResult Program::setUniform<Vec4>(Vec4&& value, const std::string &name, int count)
{
    UniformValue val;

    val.count = count;
    val.horizontalSize = 4;
    val.verticalSize = 1;
    val.value = glm::value_ptr(value);
    val.type = UniformTypes::FLOAT;

    return setUniform(name, val);
}

template<>
InvokationResult Program::setUniform<Vec2>(Vec2&& value, const std::string &name, int count)
{
    UniformValue val;

    val.count = count;
    val.horizontalSize = 2;
    val.verticalSize = 1;
    val.value = glm::value_ptr(value);
    val.type = UniformTypes::FLOAT;

    return setUniform(name, val);
}

template<>
InvokationResult Program::setUniform<Vec3>(Vec3&& value, const std::string &name, int count)
{
    UniformValue val;

    val.count = count;
    val.horizontalSize = 3;
    val.verticalSize = 1;
    val.value = glm::value_ptr(value);
    val.type = UniformTypes::FLOAT;

    return setUniform(name, val);
}

template<>
InvokationResult Program::setUniform<Mat4>(Mat4&& value, const std::string &name, int count)
{
    UniformValue val;

    val.count = count;
    val.horizontalSize = 4;
    val.verticalSize = 4;
    val.value = glm::value_ptr(value);
    val.type = UniformTypes::FLOAT;

    return setUniform(name, val);
}

template<>
InvokationResult Program::setUniform<Mat3>(Mat3&& value, const std::string &name, int count)
{
    UniformValue val;

    val.count = count;
    val.horizontalSize = 3;
    val.verticalSize = 3;
    val.value = glm::value_ptr(value);
    val.type = UniformTypes::FLOAT;

    return setUniform(name, val);
}

template<>
InvokationResult Program::setUniform<float>(float&& value, const std::string &name, int count)
{
    UniformValue val;

    val.count = count;
    val.horizontalSize = 1;
    val.verticalSize = 1;
    val.value = &value;
    val.type = UniformTypes::FLOAT;

    return setUniform(name, val);
}
