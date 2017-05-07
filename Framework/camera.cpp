#include "camera.h"

float Camera::GetNearPlane() const
{
    return nearPlane;
}

void Camera::SetNearPlane(float value)
{
    nearPlane = value;
}

float Camera::GetFarPlane() const
{
    return farPlane;
}

void Camera::SetFarPlane(float value)
{
    farPlane = value;
}

float Camera::GetAspectRatio() const
{
    return aspectRatio;
}

void Camera::SetAspectRatio(float value)
{
    aspectRatio = value;
}

Vec3 Camera::GetLookDirection() const
{
    return lookDirection;
}

void Camera::SetLookDirection(const Vec3& value)
{
    lookDirection = glm::normalize(value);
}

bool Camera::operator ==(const Camera& rhs)
{
    if (nearPlane != rhs.nearPlane)
        return false;
    if (farPlane != rhs.farPlane)
        return false;
    if (lookDirection != rhs.lookDirection)
        return false;
    if (aspectRatio != rhs.aspectRatio)
        return false;
    return GetPosition() == rhs.GetPosition();
}

Camera::Camera()
{

}

Mat4 Camera::GetCameraMatrix() const
{
    Vec3 verticalDirection = {0, 1, 0};
    if (lookDirection == -verticalDirection || lookDirection == verticalDirection)
        verticalDirection = {0, 0, 1};
    return GetCameraMatrix(lookDirection, verticalDirection);
}

Mat4 Camera::GetPerspectiveMatrix() const
{
    return glm::perspective(glm::radians(70.0f), aspectRatio, nearPlane, farPlane);
}

Mat4 Camera::GetOrthographicMatrix() const
{
    return glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, nearPlane, farPlane);
}

Mat4 Camera::GetCameraMatrix(const Vec3& lookDirection, const Vec3& verticalDirection) const
{
    return glm::lookAt(GetPosition(), GetPosition() + lookDirection, verticalDirection);
}
