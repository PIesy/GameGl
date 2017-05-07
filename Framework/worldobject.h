#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "../Math/mathdefines.h"
#include "../Graphics/graphicsclasses.h"

class WorldObject
{
    Vec3 position = {0, 0, 0};
public:
    WorldObject();
    virtual ~WorldObject();
    const Vec3& GetPosition() const;
    void SetPosition(const Vec3& value);
    bool operator ==(const WorldObject& rhs);
};

#endif // WORLDOBJECT_H
