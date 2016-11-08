#include "worldobject.h"

const Vec3& WorldObject::getPosition() const
{
    return position;
}

void WorldObject::setPosition(const Vec3& value)
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
