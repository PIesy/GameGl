//
// Created by akann on 5/3/17.
//

#ifndef GAMEGL_LIGHT_H
#define GAMEGL_LIGHT_H


#include "worldobject.h"

enum class LightType {Point, Directional, Spotlight};

class Light : public WorldObject
{
    float maxDistance = 10;
    Vec3 lightColor = {1.0f, 1.0f, 1.0f};
    Vec3 direction = {0.0f, -1.0f, 0.0f};
    float angle = 0.f;
    LightType type;
public:
    explicit Light(LightType type);
    float GetMaxDistance() const;
    const Vec3& GetLightColor() const;
    void SetLightColor(const Vec3& lightColor);
    const Vec3& GetDirection() const;
    void SetDirection(const Vec3& direction);
    float GetAngle() const;
    void SetAngle(float angle);

    std::array<Mat4, 6> GetCubeMapMatrices() const;
    Mat4 GetTranslationMatrix() const;
    Mat4 GetPerspectiveMatrix() const;
    LightType GetType() const;
};


#endif //GAMEGL_LIGHT_H
