#include "worldobject.h"

const Vec3& WorldObject::GetPosition() const
{
    return position;
}

void WorldObject::SetPosition(const Vec3& value)
{
    position = value;
}

bool WorldObject::operator ==(const WorldObject& rhs)
{
    return position == rhs.position;
}

WorldObject::WorldObject()
{

}

WorldObject::~WorldObject()
{

}
