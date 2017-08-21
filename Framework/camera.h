#ifndef CAMERA_H
#define CAMERA_H

#include "../Math/mathdefines.h"
#include "worldobject.h"

class Camera: public WorldObject
{
    float nearPlane = 1;
    float farPlane = 1;
    float aspectRatio = 1;
    Vec3 lookDirection;
public:
    Camera();

    Mat4 GetCameraMatrix() const;
    Mat4 GetCameraMatrix(const Vec3& lookDirection, const Vec3& verticalDirection) const;
    Mat4 GetPerspectiveMatrix() const;
    Mat4 GetOrthographicMatrix() const;
    float GetNearPlane() const;
    void SetNearPlane(float value);
    float GetFarPlane() const;
    void SetFarPlane(float value);
    float GetAspectRatio() const;
    void SetAspectRatio(float value);
    Vec3 GetLookDirection() const;
    void SetLookDirection(const Vec3& value);
    void LookAt(const Vec3& point);

    bool operator ==(const Camera& rhs);
};

#endif // CAMERA_H
