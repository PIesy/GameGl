#include "camera.h"
#include "Math/mathhelpers.h"

float Camera::getNearPlane() const
{
    return nearPlane;
}

void Camera::setNearPlane(float value)
{
    nearPlane = value;
}

float Camera::getFarPlane() const
{
    return farPlane;
}

void Camera::setFarPlane(float value)
{
    farPlane = value;
}

float Camera::getAspectRatio() const
{
    return aspectRatio;
}

void Camera::setAspectRatio(float value)
{
    aspectRatio = value;
}

Vec3 Camera::getLookDirection() const
{
    return lookDirection;
}

void Camera::setLookDirection(const Vec3& value)
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
    if (getPosition() != rhs.getPosition())
        return false;
    return true;
}

Camera::Camera()
{

}

Mat4 Camera::GetCameraMatrix()
{
    Vec3 verticalDirection = {0, 1, 0};

    Vec3 rightDirection = glm::normalize(glm::cross(lookDirection, verticalDirection));
    Vec3 camVerticalDirection = glm::cross(rightDirection, lookDirection);

    Mat4 rot(1.0f);
    rot[0] = Vec4(rightDirection, 0.0f);
    rot[1] = Vec4(camVerticalDirection, 0.0f);
    rot[2] = Vec4(-lookDirection, 0.0f);
    rot = glm::transpose(rot);

    Mat4 translationMat(1.0f);
    translationMat[3] = Vec4(-getPosition(), 1.0f);
    return rot * translationMat;
}

Mat4 Camera::GetPerspectiveMatrix()
{
    return PerspectiveMatrix(aspectRatio, 70.0f, nearPlane, farPlane);
}
