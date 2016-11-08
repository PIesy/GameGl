#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "Math/mathdefines.h"
#include "Graphics/graphicsclasses.h"

class WorldObject
{
    Vec3 position;
public:
    WorldObject();
    virtual ~WorldObject();
    const Vec3& getPosition() const;
    void setPosition(const Vec3& value);
    bool operator ==(const WorldObject& rhs);
};

#endif // WORLDOBJECT_H
