//
// Created by akann on 5/3/17.
//

#include "light.h"
#include <glm/gtx/component_wise.hpp>

std::array<Mat4, 6> Light::GetCubeMapMatrices() const
{
    static const int SIZE = 6;
    std::array<Mat4, SIZE> matrices;
    std::array<Vec3, SIZE> directions = {Vec3{1, 0, 0}, Vec3{-1, 0, 0}, Vec3{0, 1, 0}, Vec3{0, -1, 0}, Vec3{0, 0, 1}, Vec3{0, 0, -1}};

    for (int i = 0; i < SIZE; ++i)
    {
        Vec3 up = {0, -1, 0};
        if (i == 2)
            up = {0, 0, 1};
        else if (i == 3)
            up = {0, 0, -1};

        matrices[i] = glm::lookAt(GetPosition(), GetPosition() + directions[i], up);
    }

    return matrices;
}

const Vec3& Light::GetLightColor() const
{
    return lightColor;
}

void Light::SetLightColor(const Vec3& lightColor)
{
    Light::lightColor = lightColor;
    maxDistance = std::sqrt(glm::compMax(lightColor) / 0.01f);
}

Mat4 Light::GetPerspectiveMatrix() const
{
    return glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, maxDistance);
}

float Light::GetMaxDistance() const
{
    return maxDistance;
}
