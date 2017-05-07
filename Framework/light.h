//
// Created by akann on 5/3/17.
//

#ifndef GAMEGL_LIGHT_H
#define GAMEGL_LIGHT_H


#include "worldobject.h"

class Light : public WorldObject
{
    float maxDistance = 10;
    Vec3 lightColor = {1.0f, 1.0f, 1.0f};
public:
    float GetMaxDistance() const;
    const Vec3& GetLightColor() const;
    void SetLightColor(const Vec3& lightColor);

    std::array<Mat4, 6> GetCubeMapMatrices() const;
    Mat4 GetPerspectiveMatrix() const;
};


#endif //GAMEGL_LIGHT_H
