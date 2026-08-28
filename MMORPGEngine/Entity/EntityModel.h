#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include <MMORPGEngine/Entity/EntityOrientationModel.h>
#include <MMORPGEngine/Entity/EntityPositionModel.h>
#include <MMORPGEngine/Entity/EntitySizeModel.h>

namespace Engine {

class EntityModel {
public:
    EntityModel();
    ~EntityModel();

    EntityOrientationModel orientation() const;
    void setOrientation( const EntityOrientationModel& orientation );

    EntityPositionModel position() const;
    void setPosition( const EntityPositionModel& position );

    EntitySizeModel size() const;
    void setSize( const EntitySizeModel& size );

private:
    EntityOrientationModel _orientation;
    EntityPositionModel _position;
    EntitySizeModel _size;
};

} // namespace Engine

#endif // ENTITYMODEL_H
