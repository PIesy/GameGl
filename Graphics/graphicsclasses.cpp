#include "graphicsclasses.h"

template<>
void Shader::SetUniform<Vec4>(const Vec4& value, const std::string &name, int count)
{
    UniformValue val;

    val.count = count;
    val.horizontalSize = 4;
    val.verticalSize = 1;
    val.value = &value;
    val.type = UniformTypes::FLOAT;

    SetUniform(name, val);
}

template<>
void Shader::SetUniform<Vec2>(const Vec2& value, const std::string &name, int count)
{
    UniformValue val;

    val.count = count;
    val.horizontalSize = 2;
    val.verticalSize = 1;
    val.value = &value;
    val.type = UniformTypes::FLOAT;

    SetUniform(name, val);
}

template<>
void Shader::SetUniform<Vec3>(const Vec3& value, const std::string &name, int count)
{
    UniformValue val;

    val.count = count;
    val.horizontalSize = 3;
    val.verticalSize = 1;
    val.value = &value;
    val.type = UniformTypes::FLOAT;

    SetUniform(name, val);
}

template<>
void Shader::SetUniform<Mat4>(const Mat4& value, const std::string &name, int count)
{
    UniformValue val;

    val.count = count;
    val.horizontalSize = 4;
    val.verticalSize = 4;
    val.value = &value;
    val.type = UniformTypes::FLOAT;

    SetUniform(name, val);
}

template<>
void Shader::SetUniform<Mat3>(const Mat3& value, const std::string &name, int count)
{
    UniformValue val;

    val.count = count;
    val.horizontalSize = 3;
    val.verticalSize = 3;
    val.value = &value;
    val.type = UniformTypes::FLOAT;

    SetUniform(name, val);
}

template<>
void Shader::SetUniform<float>(const float& value, const std::string &name, int count)
{
    UniformValue val;

    val.count = count;
    val.horizontalSize = 1;
    val.verticalSize = 1;
    val.value = &value;
    val.type = UniformTypes::FLOAT;

    SetUniform(name, val);
}

template<>
void Shader::SetUniform<int>(const int& value, const std::string &name, int count)
{
    UniformValue val;

    val.count = count;
    val.horizontalSize = 1;
    val.verticalSize = 1;
    val.value = &value;
    val.type = UniformTypes::INTEGER;

    SetUniform(name, val);
}

template<>
void Shader::SetUniform<bool>(const bool& value, const std::string &name, int count)
{
    UniformValue val;
    const bool* values = &value;
    std::vector<int> convertedValues;

    for (int i = 0; i < count; i++)
        convertedValues.push_back((int)values[i]);

    val.count = count;
    val.horizontalSize = 1;
    val.verticalSize = 1;
    val.value = convertedValues.data();
    val.type = UniformTypes::INTEGER;

    SetUniform(name, val);
}