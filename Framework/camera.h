#ifndef CAMERA_H
#define CAMERA_H

#include "../Math/mathdefines.h"
#include "worldobject.h"

class Camera: public WorldObject
{
    float nearPlane;
    float farPlane;
    float aspectRatio;
    Vec3 lookDirection;
public:
    Camera();
    Mat4 GetCameraMatrix();
    Mat4 GetPerspectiveMatrix();
    float getNearPlane() const;
    void setNearPlane(float value);
    float getFarPlane() const;
    void setFarPlane(float value);
    float getAspectRatio() const;
    void setAspectRatio(float value);
    Vec3 getLookDirection() const;
    void setLookDirection(const Vec3& value);

    bool operator ==(const Camera& rhs);
};

#endif // CAMERA_H
